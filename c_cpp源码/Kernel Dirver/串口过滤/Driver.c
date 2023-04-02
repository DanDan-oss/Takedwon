#include "Dirver.h"

#define CCP_MAX_COM 32
static PDEVICE_OBJECT s_fltObj[CCP_MAX_COM] = { 0 };		// 过滤设备
static PDEVICE_OBJECT s_nextObj[CCP_MAX_COM] = { 0 };		// 串口设备引用

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	NTSTATUS nStatus = STATUS_UNSUCCESSFUL;
	if (!RegistryPath)
		return nStatus;

	if (!DriverObject)
		return nStatus;

	DbgPrint("[Message]: 驱动加载成功");

	DriverObject->DriverUnload = DriverUnload;

	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
		DriverObject->MajorFunction[i] = ccpDispatch;

	// 获取串口并绑定
	ccpAttachAllComs(DriverObject);

	nStatus = STATUS_SUCCESS;
	return nStatus;
}

void DriverUnload(PDRIVER_OBJECT DriverObject)
{
	DbgPrint("[Message]: 驱动正在进入卸载");

	LARGE_INTEGER Interval;			// __int64
	Interval.QuadPart = 3 * 1000 * ((-10) * 1000);

	UNREFERENCED_PARAMETER(DriverObject);
	for (int i = 0; i < CCP_MAX_COM; i++)
		if (s_nextObj[i])
		{
			IoDetachDevice(s_nextObj[i]);
			s_nextObj[i] = NULL;
			DbgPrint("[Message]: 删除串口对象引用%u", i);
		}

	KeDelayExecutionThread(KernelMode, FALSE, &Interval);

	for (int i = 0; i < CCP_MAX_COM; i++)
		if (s_fltObj[i])
		{
			IoDeleteDevice(s_fltObj[i]);
			s_fltObj[i] = NULL;
			DbgPrint("[Message]: 卸载过滤设备%u", i);
		}

	return;
}

// 创建虚拟设备并绑定
NTSTATUS ccpAttachDevice(PDRIVER_OBJECT Driver, PDEVICE_OBJECT oldObj, OUT PDEVICE_OBJECT* fltObj, OUT PDEVICE_OBJECT* next)
{
	NTSTATUS nStatus = STATUS_UNSUCCESSFUL;
	PDEVICE_OBJECT topDev = NULL;

	// 生成新的虚拟设备并绑定
	nStatus = IoCreateDevice(Driver, 0, NULL, oldObj->DeviceType, 0, FALSE, fltObj);
	if (nStatus != STATUS_SUCCESS)
		return nStatus;

	// 拷贝重要的标志

	if (oldObj->Flags & DO_BUFFERED_IO)
		(*fltObj)->Flags |= DO_BUFFERED_IO;
	if (oldObj->Flags & DO_DIRECT_IO)
		(*fltObj)->Flags |= DO_DIRECT_IO;
	if (oldObj->Characteristics & FILE_DEVICE_SECURE_OPEN)
		(*fltObj)->Characteristics |= FILE_DEVICE_SECURE_OPEN;

	(*fltObj)->Flags |= DO_POWER_PAGABLE;

	// 将一个虚拟设备绑定到另一个设备上
	topDev = IoAttachDeviceToDeviceStack(*fltObj, oldObj);
	if (topDev == NULL)
	{
		IoDeleteDevice(*fltObj);
		*fltObj = NULL;
		return nStatus;
	}
	*next = topDev;

	// 设置这个设备已经启动
	(*fltObj)->Flags = (*fltObj)->Flags & ~DO_DEVICE_INITIALIZING;
	return STATUS_SUCCESS;
}

// 获取一个端口设备(根据输入的ID打开相应的串口设备)
PDEVICE_OBJECT ccpOpenCom(ULONG id, NTSTATUS* status)
{
	UNREFERENCED_PARAMETER(id);
	UNICODE_STRING nameStr;
	static WCHAR name[32] = { 0 };
	PFILE_OBJECT fileObj = NULL;
	PDEVICE_OBJECT devObj = NULL;

	// 根据ID转换需要打开的串口的名字
	memset(name, 0, sizeof(WCHAR) * 32);
	RtlStringCchPrintfW(name, 32, L"\\Devices\\COM%d", id);
	RtlInitUnicodeString(&nameStr, name);
	*status = IoGetDeviceObjectPointer(&nameStr, FILE_ALL_ACCESS, &fileObj, &devObj);

	// 不需要文件对象,删除文件对象引用
	if (NT_SUCCESS(*status))
		ObDereferenceObject(fileObj);
	else
		DbgPrint("[Message]: 串口设备对象%u,[%ws]获取失败 ErrCode[%x], ", id, name, *status);

	return devObj;
}

// 获取计算机所有串口设备并绑定虚拟过滤设备
void ccpAttachAllComs(PDRIVER_OBJECT Driver)
{
	PDEVICE_OBJECT com_obj = NULL;
	NTSTATUS nStatus = STATUS_UNSUCCESSFUL;
	ULONG i = 0;
	for (i = 0; i < CCP_MAX_COM; i++)
	{
		// 获取串口对象的引用
		com_obj = ccpOpenCom(i, &nStatus);
		if (!com_obj)
			continue;
		// // 创建虚拟设备并绑定
		nStatus = ccpAttachDevice(Driver, com_obj, &s_fltObj[i], &s_nextObj[i]);
		if (nStatus == STATUS_SUCCESS)
			DbgPrint("[Message]: 获取串口成功,绑定串口%u成功", i);
		else
			DbgPrint("[Message]: 获取串口成功,绑定串口%u失败", i);
	}
	return;
}

NTSTATUS ccpDispatch(PDEVICE_OBJECT device, PIRP Irp)
{
	PIO_STACK_LOCATION irpsp = IoGetCurrentIrpStackLocation(Irp);
	ULONG32 Length = 0;						// 如果是写数据,数据大小
	PUCHAR pucBuf = NULL;					// 指向写数据缓冲区
	for (int i = 0; i < CCP_MAX_COM; i++)
	{
		if (device != s_fltObj[i])
			continue;

		// IRP为 电源操作
		if (irpsp->MajorFunction == IRP_MJ_POWER)
		{
			PoStartNextPowerIrp(Irp);
			IoSkipCurrentIrpStackLocation(Irp);
			return PoCallDriver(s_nextObj[i], Irp);
		}

		// IRP为 传送数据到设备
		if (irpsp->MajorFunction == IRP_MJ_WRITE)
		{
			// 设备缓冲区长度
			Length = irpsp->Parameters.Write.Length;
			if (Irp->MdlAddress != NULL)
				pucBuf = MmGetSystemAddressForMdlSafe(Irp->MdlAddress, NormalPagePriority);
			else
				pucBuf = Irp->UserBuffer;

			if (pucBuf == NULL)
				pucBuf = Irp->AssociatedIrp.SystemBuffer;

			for (ULONG32 j = 0; j < Length; j++)
				DbgPrint("[Messgae]: comcap(%u) send Data: %2x\r\n", i, pucBuf[j]);
		}

		IoSkipCurrentIrpStackLocation(Irp);
		return IoCallDriver(s_nextObj[i], Irp);
	}

	// 要送给的设备不在驱动对象绑定的虚拟设备中,往上层返回参数错误
	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
	IofCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;

	//ObReferenceObjectByName()
}
