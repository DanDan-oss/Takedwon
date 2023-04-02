#include "Driver.h"

VOID DriverUnLoad(PDRIVER_OBJECT pDriver)
{
	IoDeleteDevice(DeviceObject);
	IoDeleteSymbolicLink(&SymLinkName);
	DbgPrint("Delete Driver OK! \n");
}
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriver, PUNICODE_STRING RegistrPath)
{
	NTSTATUS status;
	pDriver->DriverUnload = DriverUnLoad;
	RtlInitUnicodeString(&DeviceName, L"\\Device\\HTest");
	status = IoCreateDevice(pDriver, 0, &DeviceName, FILE_DEVICE_UNKNOWN, 0, NULL, &DeviceObject);
	if (!NT_SUCCESS(status))
	{
		DbgPrint(" ���������豸ʧ��!\n");
		return STATUS_UNSUCCESSFUL;
	}
	RtlInitUnicodeString(&SymLinkName, L"\\??\\Test");		/* ��ʼ��һ���������� */
	status = IoCreateSymbolicLink(&SymLinkName, &DeviceName);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("�����豸��������ʧ��!\n");
		return STATUS_UNSUCCESSFUL;
	}
	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		pDriver->MajorFunction[i] = DisptchRoutine;
	}
	DbgPrint("Load Driver OK  %d!\n" ,TEST);
	return STATUS_SUCCESS;
}

NTSTATUS DisptchRoutine(PDEVICE_OBJECT pDevice, PIRP pIrp)
{
	pUserData TempData;
	DWORD32 A;
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation;
	ULONG InPutLen = stack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG Code = stack->Parameters.DeviceIoControl.IoControlCode;
	switch (Code)
	{
	case TEST:
		TempData = ExAllocatePoolWithTag(PagedPool, sizeof(UserData), "Tagl");
		RtlCopyMemory(TempData, stack->Parameters.DeviceIoControl.Type3InputBuffer, sizeof(UserData));
		DbgPrint("PID:%d Address:%d Type:%d dwSize:%d", TempData->ProcessId, TempData->Address, TempData->Type, TempData->dwSize);
		A = 666;
		RtlCopyMemory(pIrp->UserBuffer, &A, sizeof(DWORD32));
		break;
	}

}