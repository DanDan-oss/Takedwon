#pragma once
#include <ntddk.h>

#include <ntdddisk.h>

#pragma warning(default:4201)
#include <wdf.h>
#define NTSTRASFE_LIB
#include <ntstrsafe.h>

#define NT_DEVICE_NAME L"\\Device\\Ramdisk"
#define DOS_DEVICE_NAME L"\\DosDevices\\"

#define DRIVER_LETTER_BUFFER_SIZE 10
#define DOS_DEVNAME_BUFFER_SIZE ((sizeof(DOS_DEVICE_NAME)/2)+10)

//#define RAMDISK_TAG                     'DmaR'  // "Ram"
//#define DOS_DEVNAME_LENGTH              (sizeof(DOS_DEVICE_NAME)+sizeof(WCHAR)*10)
//#define DRIVE_LETTER_LENGTH             (sizeof(WCHAR)*10)
//#define RAMDISK_MEDIA_TYPE              0xF8
//#define DIR_ENTRIES_PER_SECTOR          16



typedef struct _DISK_INFO		// 自定义磁盘信息结构,用于存放注册表
{
	ULONG DiskSize;					// 磁盘大小
	ULONG RootDirEntries;			// 磁盘上根文件系统的进入节点
	ULONG SectorsPerCluster;		// 磁盘的每个簇由多少个扇区组成
	UNICODE_STRING DriverLetter;	// 盘符
}DISK_INFO, * PDISK_INFO;

typedef struct _DEVICE_EXTENSION
{
	PUCHAR DiskImage;				// 指向内存盘存储数据的空间
	DISK_GEOMETRY DiskGeometry;		// 磁盘几何结构(扇区)
	DISK_INFO DiskRegInfo;			// 自定义磁盘信息结构,安装时存放在注册表
	UNICODE_STRING SymbolicLink;	// 盘的符号链接名
	WCHAR DriverLetterBuf[DRIVER_LETTER_BUFFER_SIZE];		// DISK_INFO->DriverLetter的存储空间,这是用户在注册表中指定的盘符
	WCHAR DosDevNameBuf[DOS_DEVNAME_BUFFER_SIZE];			// SymbolicLink的存储空间

}DEVICE_EXTENSION,*PDEVICE_EXTENSION;



NTSTATUS RamDiskEvtDeviceAdd(WDFDRIVER Driver, PWDFDEVICE_INIT DeviceInit);		// WDF驱动添加设备
void RamDiskQueryDiskRegParameters(PWSTR DriverRegistryPath, PDISK_INFO DiskRegInfo);	
