#include "Dirver.h"




NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistrPath)
{
	NTSTATUS nStatus = STATUS_SUCCESS;
	WDF_DRIVER_CONFIG Config;
	WDF_DRIVER_CONFIG_INIT(&Config, RamDiskEvtDeviceAdd);
	nStatus = WdfDriverCreate(DriverObject, RegistrPath, WDF_NO_OBJECT_ATTRIBUTES, &Config, WDF_NO_HANDLE);
	if (NT_SUCCESS(nStatus))
		DbgPrint("[Message]; The Driver start ok! -->%wZ \n", RegistrPath);
	return nStatus;
}


NTSTATUS RamDiskEvtDeviceAdd(WDFDRIVER Driver, PWDFDEVICE_INIT DeviceInit)
{
	UNREFERENCED_PARAMETER(Driver);
	NTSTATUS nStatus = STATUS_SUCCESS;

	WDF_OBJECT_ATTRIBUTES DeviceAttr = { 0 };		// 要建立的设备对象属性
	WDFDEVICE Device = NULL;						// 要建立的设备
	PDEVICE_EXTENSION pDevExtsion = NULL;				// 设备对应的设备扩展域指针

	WDFQUEUE wdfQueue = NULL;						// 要建立的队列
	WDF_OBJECT_ATTRIBUTES queueAttr = { 0 };		// 要建立的队列对象的属性
	WDF_IO_QUEUE_CONFIG ioQueueConfig = { 0 };		// 要建立的队列的配置
	//PQUEUE_EXTENSION pQueueContext = NULL;			// 要建立的队列扩展域指针

	DECLARE_CONST_UNICODE_STRING(ntDeviceName, NT_DEVICE_NAME);			// 初始化设备名 为常量字符串

	PAGED_CODE();

	// TODO: 设置 设备名字, 类型, IRP内存缓冲区访问权限, 设备是否独占, 设置设备的设备扩展
	nStatus = WdfDeviceInitAssignName(DeviceInit, &ntDeviceName);
	if (!NT_SUCCESS(nStatus))
		return nStatus;
	WdfDeviceInitSetDeviceType(DeviceInit, FILE_DEVICE_DISK);
	WdfDeviceInitSetIoType(DeviceInit, WdfDeviceIoDirect);
	WdfDeviceInitSetExclusive(DeviceInit, FALSE);
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&DeviceAttr, _DEVICE_EXTENSION);

	//// TODO: 设置设备对象属性中的回调例程
	//DeviceAttr.EvtCleanupCallback = RamDiskEvtDevContextCleanUP;

	// TODO: 创建新设备,并获取它的设备扩展
	nStatus = WdfDeviceCreate(&DeviceInit, &DeviceAttr, &Device);
	if (NT_SUCCESS(nStatus))
		return nStatus;
	//pDevExtsion = DeviceGetExtension(Device);

	// TODO: 初始化WDF驱动消息队列结构, 指定队列的对象扩展
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQueueConfig, WdfIoQueueDispatchSequential);
	//ioQueueConfig.EvtIoDeviceControl = RamDiskEvtIoDeviceControl;
	//ioQueueConfig.EvtIoRead = RamDiskEvtIoRead;
	//ioQueueConfig.EvtIoWrite = RamDiskEvtIoWrite;

	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&queueAttr, _QUEUE_EXTENSION);

	nStatus = WdfIoQueueCreate(Device, &ioQueueConfig, &queueAttr, &wdfQueue);
	//pQueueContext = QueueGetExtension(wdfQueue);
	//pQueueContext->DeviceExtension=pDeviceExtension

	pDevExtsion->DiskRegInfo.DriverLetter.Buffer = (PWSTR)&pDevExtsion->DriverLetterBuf;
	pDevExtsion->DiskRegInfo.DriverLetter.MaximumLength = sizeof(pDevExtsion->DriverLetterBuf);
	RamDiskQueryDiskRegParameters(WdfDriverGetRegistryPath(WdfDeviceGetDriver(Device)), &pDevExtsion->DiskRegInfo);

	return nStatus;
}

void RamDiskQueryDiskRegParameters(PWSTR DriverRegistryPath, PDISK_INFO DiskRegInfo)
{
	
}