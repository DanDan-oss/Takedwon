#pragma once
#include <ntddk.h>

#define TEST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x666, METHOD_NEITHER, FILE_ANY_ACCESS)

UNICODE_STRING DeviceName;
UNICODE_STRING SymLinkName;
PDEVICE_OBJECT DeviceObject;

typedef struct _UserData
{
	DWORD32 ProcessId;
	DWORD32 Address;
	DWORD32 Type;
	DWORD32 dwSize;
	PVOID InPutBuffer;
}UserData,*pUserData;

VOID DriverUnLoad(PDRIVER_OBJECT pDriver);
NTSTATUS DisptchRoutine(PDEVICE_OBJECT pDevice, PIRP pIrp);
