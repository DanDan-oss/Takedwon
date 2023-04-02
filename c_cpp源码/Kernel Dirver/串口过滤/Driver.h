#pragma once

#include <ntddk.h>
#define NTSTRSAFE_LIB
#include <ntstrsafe.h>



void DriverUnload(PDRIVER_OBJECT DriverObject);		// 驱动卸载回调函数

PDEVICE_OBJECT ccpOpenCom(ULONG id, NTSTATUS* status);	// 打开一个端口设备(根据输入的ID打开相应的窗口设备)

//创建虚拟设备并绑定到旧设备上
NTSTATUS ccpAttachDevice(PDRIVER_OBJECT Driver, PDEVICE_OBJECT oldObj, OUT PDEVICE_OBJECT* fltObj, OUT PDEVICE_OBJECT* next);

void ccpAttachAllComs(PDRIVER_OBJECT Driver);		// 获取计算机所有串口设备并绑定虚拟过滤设备

NTSTATUS ccpDispatch(PDEVICE_OBJECT device, PIRP Irp);		// 驱动分发函数

NTSTATUS ObReferenceObjectByName(PUNICODE_STRING ObjectName, ULONG Attributes, PACCESS_STATE AccessState,
    ACCESS_MASK DesiredAccess, POBJECT_TYPE ObjectType, KPROCESSOR_MODE AccessMode, PVOID ParseContext,
    PVOID* Object);
