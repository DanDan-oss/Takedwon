#include "Dirver.h"

#define _CRT_SECURE_NO_WARNINGS

#define DeviceName L"\\Device\\MyDev"
#define SymbolicLinkName L"\\??\\MyDev"

#define DevReadCtrl CTL_CODE(FILE_DEVICE_UNKNOWN, 0x911, METHOD_BUFFERED, FILE_ANY_ACCESS)

NTSTATUS(_stdcall* OldProcess)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PCLIENT_ID) = 0;

NTSTATUS _stdcall myOpenProcess(
	PHANDLE ProcessHandle,
	ACCESS_MASK DesiredAccess,
	POBJECT_ATTRIBUTES ObjectAttributes,
	PCLIENT_ID ClientId)
{
	DbgPrint("[dbg]: NtOpenProcess Handle = %p [%wZ] UniqueProcess = %p  UniqueThread=%p\n", 
		ProcessHandle, ObjectAttributes->ObjectName, ClientId->UniqueProcess, ClientId->UniqueThread);

	return OldProcess(ProcessHandle, DesiredAccess, ObjectAttributes, ClientId);
}

//extern NTSYSAPI NTSTATUS ZwOpenProcess(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PCLIENT_ID);
NTSTATUS cwkDispatch(PDEVICE_OBJECT, PIRP);			// ������IRP�ַ�����
void DriverUnload(PDRIVER_OBJECT);					// ������ж�غ���

PDEVICE_OBJECT COMDevObject = NULL;					// ͨ��
PPEB PebBaseAddress = 0;							// ��ǰ����PEB


void SampleUnload(PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);

	return;
}

 NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	DbgPrint("[dbg]The Driver Loadre\n");
	//KdPrint(("[dbg]The Driver Loadre\n"));
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	UNREFERENCED_PARAMETER(RegistryPath);
	RTL_OSVERSIONINFOW VersionInfo = { 0 };
	VersionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);

	status = RtlGetVersion(&VersionInfo);
	if (NT_SUCCESS(status))
	{
		DbgPrint("[dbg]The System Version: %d-%d-%d %x %s\n", VersionInfo.dwBuildNumber, VersionInfo.dwMajorVersion, VersionInfo.dwMinorVersion,
			VersionInfo.dwPlatformId, VersionInfo.szCSDVersion);
		//KdPrint(("[dbg]The System Version: %d-%d-%d %x %s\n", VersionInfo.dwBuildNumber, VersionInfo.dwMajorVersion, VersionInfo.dwMinorVersion,
		//	VersionInfo.dwPlatformId, VersionInfo.szCSDVersion));
	}

	DriverObject->DriverUnload = SampleUnload;
	return STATUS_SUCCESS;
}


//NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
//{
//	UNREFERENCED_PARAMETER(RegistryPath);
//	NTSTATUS nStatus = STATUS_SUCCESS;
//	DbgPrint("[dbg]: DriverObject=%p DriverStart=%p DriverExtension=%p\n", DriverObject, DriverObject->DriverStart, DriverObject->DriverExtension);
//	DriverObject->DriverUnload = DriverUnload;
//	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
//		DriverObject->MajorFunction[i] = cwkDispatch;
//
//	PVOID64 Func = SetSSDTFuncHook(0x26, (PVOID64)&myOpenProcess);
//	OldProcess =(NTSTATUS(_stdcall *)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PCLIENT_ID))Func;
//		DbgPrint("[dbg]: NtOpenProcessAddre=%p \n", OldProcess);
//
//	//shellcode();
//
//	nStatus = CreateCOMDev(DriverObject);
//	if (NT_SUCCESS(nStatus))
//		DbgPrint("[dbg]: The driver Loader Ok \n");
//	return nStatus;
//}

void DriverUnload(PDRIVER_OBJECT DriverObject)
{
	if (OldProcess)
		SetSSDTFuncHook(0X26, (PVOID64)&OldProcess);

	if (!NT_SUCCESS(UnloadCOMDev(DriverObject)))
		KdPrint(("[dbg]: Device Unload fail ----%x\n", __LINE__));
	else
		KdPrint(("[dbg]: Device Unload OK ----%x\n", __LINE__));
	return;
}

NTSTATUS cwkDispatch(PDEVICE_OBJECT dev, PIRP irp)
{
	PIO_STACK_LOCATION irpsp;
	NTSTATUS nStatus = STATUS_UNSUCCESSFUL;
	if (dev == COMDevObject)
	{
		nStatus = STATUS_SUCCESS;
		irpsp = IoGetCurrentIrpStackLocation(irp);
		switch (irpsp->MajorFunction)
		{
		case  IRP_MJ_CREATE:
			DbgPrint("[dbg]: ��⵽Ӧ�ò������������ \n");
			break;

		case  IRP_MJ_CLOSE:
			DbgPrint("[dbg]: ��⵽Ӧ�ò�رշ����������� \n");
			break;

		case IRP_MJ_DEVICE_CONTROL:
			if (irpsp->Parameters.DeviceIoControl.IoControlCode == DevReadCtrl)
			{
				DbgPrint("[dbg]: ��⵽Ӧ�ò��豸��������SSDT Hook \n");
			}
			
			break;
		default:
			break;
		}
	}
	

	// IRP��Ϣ �Ѵ���
	irp->IoStatus.Information = 0;
	irp->IoStatus.Status = nStatus;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return nStatus;
	
}

NTSTATUS CreateCOMDev(PDRIVER_OBJECT DriverObject)
{
	NTSTATUS nStatus = STATUS_UNSUCCESSFUL;
	PDEVICE_OBJECT DevObject = NULL;

	UNICODE_STRING DevName = { 0 };
	UNICODE_STRING SymboLinkName = { 0 };

	RtlInitUnicodeString(&DevName, DeviceName);
	RtlInitUnicodeString(&SymboLinkName, SymbolicLinkName);

	nStatus = IoCreateDevice(DriverObject, 0, &DevName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN,
		FALSE, &DevObject);
	if (!NT_SUCCESS(nStatus))
	{
		KdPrint(("[dbg]: The Driver create fail ----%x\n", __LINE__));
		return nStatus;
	}
	DevObject->Flags |= DO_BUFFERED_IO;

	nStatus = IoCreateSymbolicLink(&SymboLinkName, &DevName);
	if (!NT_SUCCESS(nStatus))
	{
		KdPrint(("[dbg]: The Device create ymbolicLink fail ----%x\n", __LINE__));
		IoDeleteDevice(DevObject);
		return nStatus;
	}
	COMDevObject = DevObject;
	return nStatus;
}

NTSTATUS UnloadCOMDev(PDRIVER_OBJECT DriverObject)
{
	NTSTATUS nStatus = STATUS_UNSUCCESSFUL;
	UNICODE_STRING SymboLinkName = { 0 };

	RtlInitUnicodeString(&SymboLinkName, SymbolicLinkName);
	nStatus = IoDeleteSymbolicLink(&SymboLinkName);
	if (!NT_SUCCESS(nStatus))
		KdPrint(("[dbg]: Device SymbolicLink delete fail ----%x\n", __LINE__));
	IoDeleteDevice(DriverObject->DeviceObject);
	return nStatus;
}


PVOID64 GetSSDTAddre()
{
	PUCHAR msr = 0;
	PUCHAR StartAddre = 0, EndAddre = 0;			// ��ʼ�����ĵط�,�ͱ�������,��ȡ��SSDT��λ��
	//UCHAR b0 = 0, b1 = 0, b2 = 0, b7 = 0, b8 = 0, b9 = 0 ,b14 = 0, b15=0;
	ULONG deviation = 0;									//KiSystemServiceRepeat+7��SSDT��ƫ��
	ULONGLONG SSDTAddre = 0;

	msr = (PUCHAR)__readmsr(0xC0000082);
	// __readmsr(0xC0000082)�� �����ں˸��������»�ȡ������KiSystemCall64Shadow����,δ�����ں˸������ KiSystemCall64
	// ͨ��KiSystemCall64Shadow/ KiSystemCall64��λ�� KiSystemServiceRepeat����,KiSystemServiceRepeat
	if (msr[21] == 0x72)
	{
		// ϵͳδ�����ں˸���ģʽ KiSystemCall64���±���  �������롾4C 8D 15 XX XX XX XX 4C 8D 1D XX XX XX XX F7 43���ҵ�KiSystemServiceRepeat
		// �ڶ��� 4C 8D 1D XX XX XX XX ��Shadow SSDT��ַ
		StartAddre = msr, EndAddre = msr + 0x500;
		for (; StartAddre < EndAddre; StartAddre++)
			if (MmIsAddressValid(StartAddre) && MmIsAddressValid(StartAddre + 1) && MmIsAddressValid(StartAddre + 2))
			{
				char* p = (char* )StartAddre;
				if (p[0] == 0x4C && p[1] == 0x8d && p[2] == 0x15 && p[7] == 0x4C && p[8] == 0x8d && p[9] == 0x1D && p[14] == 0xF7 && p[15] == 0x43)
				{
					memcpy(&deviation, StartAddre + 3, 4);
					SSDTAddre = (ULONGLONG)deviation + (ULONGLONG)StartAddre + 7;
					KdPrint(("[dbg]: KiSystemCall64:=%p KiSystemServiceRepeat=%p SSDT=%p\n", msr, StartAddre, (PULONG64)SSDTAddre));
					return (PVOID64)SSDTAddre;
				}
			}
	}
	else if (msr[21] == 0x65)
	{
		// ϵͳ���� �ں˸���ģʽ ͨ�� KiSystemCall64Shadow������ �����롾4C 8D 15 XX XX XX XX 4C 8D 1D XX XX XX XX F7 43���ҵ� KiSystemServiceRepeat
		// ��һ��4C 8D 1D XX XX XX XX ��SSDT  �ڶ��� 4C 8D 1D XX XX XX XX ��Shadow SSDT��ַ
		StartAddre = msr - 0x200000, EndAddre = msr;
		for (; StartAddre < EndAddre; StartAddre++)
			if (MmIsAddressValid(StartAddre) && MmIsAddressValid(StartAddre + 1) && MmIsAddressValid(StartAddre + 2))
			{
				char* p = (char*)StartAddre;
				if (p[0] == 0x4C && p[1] == 0x8d && p[2] == 0x15 && p[7] == 0x4C && p[8] == 0x8d && p[9] == 0x1D && p[14] == 0xF7 && p[15] == 0x43)
				{
					memcpy(&deviation, StartAddre + 3, 4);
					SSDTAddre = (ULONGLONG)deviation + (ULONGLONG)StartAddre + 7;
					KdPrint(("[dbg]: KiSystemCall64Shadow=%p KiSystemServiceRepeat=%p SSDT=%p\n", msr, StartAddre, (PULONG64)SSDTAddre));
					return (PVOID64)SSDTAddre;
				}
			}
	}

	DbgPrint("[dbg]: No find SSDT\n");
	return NULL;
}


PVOID64 GetSSDTFuncAddre(DWORD64 FuncIndex)
{
	PKSYSTEM_SERVICE_TABLE SSDT = 0; 
	PULONGLONG FuncAddre = 0;
	ULONG dwOffset = 0;

	SSDT = GetSSDTAddre();
	if (SSDT)
	{
		dwOffset = *((PULONG)SSDT->ServiceTableBase + FuncIndex);
		if (dwOffset & 0x80000000)
			dwOffset = dwOffset >> 4 | 0xF0000000;
		else
			dwOffset = dwOffset >> 4;
		FuncAddre = (PULONGLONG)((ULONGLONG)SSDT->ServiceTableBase + (ULONGLONG)dwOffset);

		DbgPrint("[dbg]: SSDTFuncAddre[%lld] [%p] = ServiceTableBase[%p] + dwOffset[%lx]\n", FuncIndex, (PVOID64)FuncAddre, SSDT->ServiceTableBase, dwOffset);
		return FuncAddre;
	}

	DbgPrint("[dbg]:  No find SSDT Function [%lld]  ", FuncIndex);
	return 0;
}

PVOID64 SetSSDTFuncHook(DWORD64 FuncIndex, PVOID64 FuncAddre)
{
	PKSYSTEM_SERVICE_TABLE pSSDT = 0;
	PULONGLONG OldFuncAddre = 0;
	ULONG dwOffset = 0;
	ULONG oldOffset = 0;
	pSSDT = GetSSDTAddre();
	if (pSSDT)
	{
		oldOffset = *((PULONG)pSSDT->ServiceTableBase + FuncIndex);
		if (oldOffset & 0x80000000)
			oldOffset = oldOffset >> 4 | 0xF0000000;
		else
			oldOffset = oldOffset >> 4;
		OldFuncAddre = (PULONGLONG)((ULONGLONG)pSSDT->ServiceTableBase + (ULONGLONG)oldOffset);
		KdPrint(("[dbg]: SSDT OldFunction Addre[%p]\n",GetSSDTFuncAddre(FuncIndex)));
		dwOffset = (ULONG)((ULONGLONG)FuncAddre - (ULONGLONG)pSSDT->ServiceTableBase);
		dwOffset = dwOffset << 4;
		if (!(dwOffset & 0xF0000000))
			dwOffset = dwOffset | 0x80000000;
		*((PULONG)pSSDT->ServiceTableBase + FuncIndex) = dwOffset;
		DbgPrint("[dbg]:  SetSSDT office = %ld  Function[%p]==[%p] \n", dwOffset, GetSSDTFuncAddre(FuncIndex), FuncAddre);
		return OldFuncAddre;
	}
	return 0;
}

//void __declspec shellcode()
//{
//	int ErrorCode = 0;
//	
//	//PLIST_ENTRY64 Liststruct = { 0 };		// ����ṹ��
//	
//	PebBaseAddress = (PPEB)__readfsdword(0x30);   //FS[0x60]  ��x86����PEB
//	ErrorCode = GetLastError();
//
//	DbgPrint("[dbg]: PebBaseAddress: [%x] \n", PebBaseAddress);
//	PPEB_LDR_DATA pPebLdr = PebBaseAddress->pLdr;
//	PLDR_DATA_TABLE_ENTRY pLdrDataHeader = (PLDR_DATA_TABLE_ENTRY)pPebLdr->InMemoryOrderModuleList.Flink;
//	PLDR_DATA_TABLE_ENTRY pLdrDataTail = (PLDR_DATA_TABLE_ENTRY)pPebLdr->InMemoryOrderModuleList.Flink;
//	do
//	{
//		WCHAR* DllName = pLdrDataHeader->BaseDllName.pBuffer;
//		//USHORT usCounter = pLdrDataHeader->BaseDllName.Length;
//		pLdrDataHeader = (PLDR_DATA_TABLE_ENTRY)pLdrDataHeader->InMemoryOrderModuleList.Flink;
//		DbgPrint("[dbg]: PebBaseAddress: [%s] \n", DllName);
//	} while (pLdrDataHeader != pLdrDataTail);
//
//	return 0;
//}

