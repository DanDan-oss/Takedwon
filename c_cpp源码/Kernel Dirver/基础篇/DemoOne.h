#pragma once
#include <ntifs.h>
#include <ntddk.h>
#include <wdm.h>


typedef struct _TestListEntry
{
	ULONG m_ulDataA;
	ULONG m_ulDataB;
	LIST_ENTRY m_ListEntry;
	ULONG m_ulDataC;
	ULONG m_ulDataD;
}TestListEntry, * PTestListEntry;

NTSTATUS cwkDispatch(PDEVICE_OBJECT dev, PIRP irp);		// 驱动对象的分发函数
VOID TestFirst(); 										// 测试代码
VOID TestListFunc();									// 链表
VOID DriverUnload(PDRIVER_OBJECT DriverObject);			// 驱动程序的"析构"函数
BOOLEAN UseLookasideDemoCode();							// 旁视列表
BOOLEAN EventOperationSample();							// 句柄操作
NTSTATUS DriverRegKey();								// 操作注册表
NTSTATUS DriverFile();									// 内核函数文件操作
NTSTATUS DirverThread();								// 线程测试函数
VOID CustomThreadProc(PVOID context);					// 线程回调函数
NTSTATUS ControlDevice();								// 符号链接设备的生成、链接、删除
PDEVICE_OBJECT KeCreateDevice(PDRIVER_OBJECT pDriObj, PWCHAR strDeviceName);	// 监控关机操作控制设备对象
NTSTATUS SysShutdown(PDEVICE_OBJECT DeviceObject, PIRP pIrp);	// 检测到系统系统即将关机执行的回调函数
NTSTATUS ReadShutdownIni();								// 读取配文件


