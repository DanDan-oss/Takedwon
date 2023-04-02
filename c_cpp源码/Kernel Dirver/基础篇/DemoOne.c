#include "Dirver.h"

#define ulFileBufSize 1024*4
#define usDeviceName L"\\Device\\cwk_118m7480h520"
#define usSymblName L"\\??\\slbkcdo_118m7480h520"

#define CWK_DVC_SEND_STR (ULONG)CTL_CODE(FILE_DEVICE_UNKNOWN, 0x192, METHOD_BUFFERED, FILE_READ_DATA)

PUNICODE_STRING RegPath;
PDRIVER_OBJECT pDriverObj;				// ??????
PDEVICE_OBJECT pDeviceObj = NULL;		// ?????ƶ??
PDEVICE_OBJECT g_pDevObj = NULL;		// ?ػ????豸???


VOID TestFirst()
{
	//TestListFunc();				// ???ں????
	//UseLookasideDemoCode();		// ????????б?
	//EventOperationSample();		// ??Ծ????
	//DriverRegKey();				// ?????????
	//DriverFile();					// ???ļ????
	//DirverThread();				// ???ں?߳?
	return;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{

	NTSTATUS nStatus = STATUS_UNSUCCESSFUL;

	DbgPrint("[%ws]Hello Knenel World, CurrentProcessId=0x%p, CurrentIRQL=0x%u\n", __FUNCTIONW__,
		PsGetCurrentProcessId(), KeGetCurrentIrql());
	if (!RegistryPath)
		return nStatus;

	RegPath = RegistryPath;
	DbgPrint("[%ws]Driver RegistryPath:%wZ\n", __FUNCTIONW__, RegistryPath);

	do
	{
		if (!DriverObject)
			break;

		DbgPrint("[%ws]Driver Object Address:%p\n", __FUNCTIONW__, DriverObject);
		pDriverObj = DriverObject;								// ??????????Ϊȫ?ֱ??
		DriverObject->DriverUnload = DriverUnload;				// ????????????(??????)
		for (ULONG i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)		// ??ʼ?????????ַ????,ȫ??ָ?cwkDispatch
			DriverObject->MajorFunction[i] = cwkDispatch;

		ControlDevice();										// ??ɿ??豸???

		DriverObject->MajorFunction[IRP_MJ_SHUTDOWN] = SysShutdown;
		g_pDevObj = KeCreateDevice(DriverObject, L"\\Device\\TestShutdown");
		if (!g_pDevObj)
			break;
		nStatus = IoRegisterShutdownNotification(g_pDevObj);
	} while (FALSE);
	if (nStatus != STATUS_SUCCESS)
	{
		DbgPrint("[Message]: ?????ʼ??ʧ?ܣ????ж?");
		DriverUnload(DriverObject);
	}

	ReadShutdownIni();
	return nStatus;
}

VOID DriverUnload(PDRIVER_OBJECT DriverObject)
{
	if (pDeviceObj)
	{
		UNICODE_STRING hCDO_syb = RTL_CONSTANT_STRING(L"\\??\\slbkcdo_118m7480h520");
		DbgPrint("[Message]:Unloading driver, Delete [%wZ] Device!\n", &hCDO_syb);
		IoDeleteSymbolicLink(&hCDO_syb);
		IoDeleteDevice(pDeviceObj);
		pDeviceObj = NULL;
	}

	if (DriverObject)
		DbgPrint("[Message]: [%ws]:Object Address:%p,CurentProcessId=0x%p, CurrentIRQL=0x%u\n",
			__FUNCTIONW__, DriverObject, PsGetCurrentProcessId(), KeGetCurrentIrql());

	if (g_pDevObj)
	{
		DbgPrint("[Message]: ?????ж?,?رռ?ӹػ????");
		IoUnregisterShutdownNotification(g_pDevObj);
		IoDeleteDevice(g_pDevObj);
		g_pDevObj = NULL;
	}


	return;
}

VOID TestListFunc()
{
	UNICODE_STRING uFirstString = { 0 };
	//wchar_t str[] = TEXT("Hello Kernel Linked list\n");
	RtlInitUnicodeString(&uFirstString, TEXT("Hello Kernel Linked list\n"));
	DbgPrint("[Message] [String uFirstString]:%wZ\n", &uFirstString);

	// ?ṹ????????ʼ??...
	LIST_ENTRY ListHead = { 0 };		// ͷ?ڵ?
	TestListEntry ListEntryA = { 0 };
	TestListEntry ListEntryB = { 0 };
	TestListEntry ListEntryC = { 0 };
	TestListEntry ListEntryD = { 0 };
	ListEntryA.m_ulDataA = 'A';
	ListEntryB.m_ulDataA = 'B';
	ListEntryC.m_ulDataA = 'C';
	ListEntryD.m_ulDataA = 'D';

	InitializeListHead(&ListHead);
	InsertHeadList(&ListHead, &ListEntryB.m_ListEntry);
	InsertHeadList(&ListHead, &ListEntryA.m_ListEntry);
	InsertTailList(&ListHead, &ListEntryC.m_ListEntry);
	InsertTailList(&ListHead, &ListEntryD.m_ListEntry);

	// ѭ?????????
	PLIST_ENTRY pListEntry = NULL;
	pListEntry = ListHead.Flink;
	int IndexNum = 1;
	while (pListEntry != &ListHead)
	{
		PTestListEntry pTestEntry = CONTAINING_RECORD(pListEntry, TestListEntry, m_ListEntry);
		DbgPrint("[Message] [ListEntry:%d] ListPtr=%p, Entry=%p, Tag=%c \n", IndexNum, pTestEntry, pListEntry,
			(CHAR)pTestEntry->m_ulDataA);

		pListEntry = pListEntry->Flink;
		IndexNum++;
	}
	return;
}

BOOLEAN UseLookasideDemoCode()
{
	PNPAGED_LOOKASIDE_LIST pLookAsideList = NULL;		// ???б???
	PVOID pFirstMemory = NULL;			// ָ????б?ڴ?1
	PVOID pSecondMemory = NULL;			// ָ????б?ڴ?2
	BOOLEAN bSucc = FALSE;				// ?????ִ??ɹ?
	BOOLEAN bInit = FALSE;				// ???ʼ???ɹ?
	do
	{
		pLookAsideList = ExAllocatePoolWithTag(NonPagedPool, sizeof(NPAGED_LOOKASIDE_LIST), 'test');
		if (!pLookAsideList)
		{
			DbgPrint("[Message] ERROR:???????б????!\n");
			break;
		}
		memset(pLookAsideList, 0, sizeof(NPAGED_LOOKASIDE_LIST));
		ExInitializeNPagedLookasideList(pLookAsideList, NULL, NULL, 0,
			0x128, 'test', 0);
		bInit = TRUE;
		pFirstMemory = ExAllocateFromNPagedLookasideList(pLookAsideList);
		if (!pFirstMemory)
		{
			DbgPrint("[Message] ERROR:???б??????_1??ʧ??\n");
			break;
		}
		pSecondMemory = ExAllocateFromNPagedLookasideList(pLookAsideList);
		if (!pSecondMemory)
		{
			DbgPrint("[Message] ERROR:???б??????_2??ʧ??\n");
			break;
		}

		// ???б??󴴽??ɹ?,????ɹ??????
		DbgPrint("[Message] NPaged_Lookaside_List:pFirstMemory=0x%p , pSecondMemory=0x%p\n",
			pFirstMemory, pSecondMemory);

		// ???FirstMemory????,?鿴????????Ƿ???
		ExFreeToNPagedLookasideList(pLookAsideList, pFirstMemory);
		pFirstMemory = NULL;
		pFirstMemory = ExAllocateFromNPagedLookasideList(pLookAsideList);
		if (!pFirstMemory)
		{
			DbgPrint("[Message] ERROR:pFirstMemory?????ڴ????\n");
			// KeBugCheckEx(0x0, 0x0, 0x0, 0x0, 0x0);		// ???
			break;
		}
		bSucc = TRUE;
		DbgPrint("[Message] ReAlloc:pFirstMemory=0x%p\n", pFirstMemory);
	} while (FALSE);
	if (pFirstMemory)
	{
		ExFreeToNPagedLookasideList(pLookAsideList, pFirstMemory);
		pFirstMemory = NULL;
	}
	if (pSecondMemory)
	{
		ExFreeToNPagedLookasideList(pLookAsideList, pSecondMemory);
		pSecondMemory = NULL;
	}
	if (bInit)
	{
		ExDeleteNPagedLookasideList(pLookAsideList);
		pLookAsideList = NULL;
	}
	if (pLookAsideList)
	{
		ExFreePoolWithTag(pLookAsideList, 'test');
		pLookAsideList = NULL;
	}
	return bSucc;
}

BOOLEAN EventOperationSample()
{
	BOOLEAN bSucc = FALSE;
	HANDLE hCreateEvent = NULL;
	PVOID pCreateEventObject = NULL;
	HANDLE hOpenEvent = NULL;
	PVOID pOpenEventObject = NULL;
	do
	{
		OBJECT_ATTRIBUTES ObjAttr = { 0 };
		UNICODE_STRING uNameString = { 0 };
		RtlInitUnicodeString(&uNameString, L"\\BaseNamedObjects\\TestEvent");
		InitializeObjectAttributes(&ObjAttr, &uNameString, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
			NULL, NULL);
		ZwCreateEvent(&hCreateEvent, EVENT_ALL_ACCESS, &ObjAttr, SynchronizationEvent, FALSE);
		if (!hCreateEvent)
		{
			DbgPrint("[Message] ERROR:hCreateEvent == NULL\n");
			break;
		}
		ObReferenceObjectByHandle(hCreateEvent, EVENT_ALL_ACCESS, *ExEventObjectType, KernelMode,
			&pCreateEventObject, NULL);
		if (!pCreateEventObject)
		{
			DbgPrint("[Message] ERROR:pCreateEventObject == NULL\n");
			break;
		}
		ZwOpenEvent(&hOpenEvent, EVENT_ALL_ACCESS, &ObjAttr);
		if (!hOpenEvent)
		{
			DbgPrint("[Message] ERROR:hOpenEvent == NULL\n");
			break;
		}
		ObReferenceObjectByHandle(hOpenEvent, EVENT_ALL_ACCESS, *ExEventObjectType, KernelMode,
			&pOpenEventObject, NULL);
		if (!pOpenEventObject)
		{
			DbgPrint("[Message] ERROR:pOpenEventObject == NULL\n");
			break;
		}
		DbgPrint("[Message] Create Handle: %p, Create Pointer= %p\n", hCreateEvent, pCreateEventObject);
		DbgPrint("[Message] Open Handle: %p, Open Pointer= %p\n", hOpenEvent, pOpenEventObject);
		bSucc = TRUE;

	} while (FALSE);
	if (pCreateEventObject)
	{
		ObDereferenceObject(pCreateEventObject);
		pCreateEventObject = NULL;
	}
	if (hCreateEvent)
	{
		ZwClose(hCreateEvent);
		hCreateEvent = NULL;
	}
	if (pOpenEventObject)
	{
		ObDereferenceObject(pOpenEventObject);
		pOpenEventObject = NULL;
	}
	if (hOpenEvent)
	{
		ZwClose(hOpenEvent);
		hOpenEvent = NULL;
	}

	return bSucc;
}

NTSTATUS DriverRegKey()
{
	OBJECT_ATTRIBUTES ObjAttr = { 0 };			// ???????ṹ???Ľṹ
	HANDLE hKey = NULL;							// ZwCreateKey ??ռ????ľ????
	PUNICODE_STRING RegistryPath = RegPath;		// ??ǰ????????????
	ULONG ulDisosition = 0;						// ???ص?ǰע?????????Ļ????Ѵ?ڵ?
	NTSTATUS ntStatus = 0;						// ע?????????????

	InitializeObjectAttributes(&ObjAttr, RegistryPath, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);
	ntStatus = ZwCreateKey(&hKey, KEY_WRITE, &ObjAttr, 0, NULL, REG_OPTION_NON_VOLATILE, &ulDisosition);
	if (hKey)
	{
		UNICODE_STRING usValueName = { 0 };
		RtlInitUnicodeString(&usValueName, L"Start");
		ULONG ulNewStartValue = 52;
		// ??????
		ntStatus = ZwSetValueKey(hKey, &usValueName, 0, REG_DWORD, (PVOID)ulNewStartValue, sizeof(ulNewStartValue));
		if (ntStatus == STATUS_SUCCESS)
			DbgPrint("[Message] ZwSetValueKey:Registry modified successfully !\n");
		else
		{
			if (ntStatus == STATUS_ACCESS_DENIED)
				DbgPrint("[Message] ZwSetValueKey:Registry modification failed, ///STATUS_ACCESS_DENIED!\n");
			else if (ntStatus == STATUS_INVALID_HANDLE)
				DbgPrint("[Message] ZwSetValueKey:Registry modification failed, ///STATUS_INVALID_HANDLE!\n");
			else
				DbgPrint("[Message] ZwSetValueKey:Registry modification failed, ///unknown Error!\n");
		}

		// ??ȡע??
		ULONG ulRetSize = 0;
		// ??ntStatus??ȡʱ????Ļ????ΪNULL?ʹ??Ϊ0, ?????????????̫С,ulRetSize??????Ҫ????Ϣ?Ĵ??
		ntStatus = ZwQueryValueKey(hKey, &usValueName, KeyValuePartialInformation, NULL, 0, &ulRetSize);
		if (ntStatus == STATUS_BUFFER_TOO_SMALL && ulRetSize != 0)
		{
			// DbgPrint("[ZwQueryValueKey]: Need size %u\n", ulRetSize);
			ULONG ulStartValue = 0;		// ?洢??ֵ
			PKEY_VALUE_PARTIAL_INFORMATION pData = ExAllocatePoolWithTag(PagedPool, ulRetSize, 'driF');
			if (pData)
			{
				memset(pData, 0,/*sizeof(KEY_VALUE_PARTIAL_INFORMATION)*/ulRetSize);
				ntStatus = ZwQueryValueKey(hKey, &usValueName, KeyValuePartialInformation, pData, ulRetSize, &ulRetSize);
				if (ntStatus == STATUS_SUCCESS)
				{
					ulStartValue = *(ULONG*)pData->Data;
					DbgPrint("[Message] Key:%wZ, ValueName:%wZ,Value:%u\n", RegistryPath, usValueName,
						ulStartValue);
				}
				ExFreePoolWithTag(pData, 'driF');
				pData = NULL;
			}
		}

		ZwClose(hKey);
		hKey = 0;
	}
	return STATUS_SUCCESS;
}

NTSTATUS DriverFile()
{
	PVOID pFileBuufer = NULL;				// ?洢??????Ļ????
	ULONG ulBufLen = 0;						// ?????????
	HANDLE SourceFileHandle = NULL;				// Դ?????
	HANDLE targetFileHandle = NULL;				// Ŀ??ļ???
	LARGE_INTEGER ByOffset = { 0 };				// ????Ķ?ȡλ?
	NTSTATUS Status = 0;						// ???ִ??????̬
	IO_STATUS_BLOCK IoStaBlock = { 0 };			// ?洢???ִ????????Ϣ
	UNICODE_STRING ulSourFileName = RTL_CONSTANT_STRING(L"\\??\\C:\\a.dat");	// Դ????????
	OBJECT_ATTRIBUTES sourObjAttr = { 0 };
	UNICODE_STRING ulTargFileName = RTL_CONSTANT_STRING(L"\\??\\C:\\b.dat");	// Ŀ??ļ?·??
	OBJECT_ATTRIBUTES targObjAttr = { 0 };					// Ŀ??ļ???

	// ??ʼ?????ṹ?Ϣ
	InitializeObjectAttributes(&sourObjAttr, &ulSourFileName, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);
	InitializeObjectAttributes(&targObjAttr, &ulTargFileName, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);
	do
	{
		// ??ļ?
		Status = ZwCreateFile(&SourceFileHandle, FILE_WRITE_DATA | FILE_READ_DATA, &sourObjAttr, &IoStaBlock, 0,
			FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN_IF,
			FILE_NON_DIRECTORY_FILE | FILE_RANDOM_ACCESS | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
		if (Status != STATUS_SUCCESS)
		{
			DbgPrint("[mohui] Error:Open Source File !\n");
			break;
		}
		Status = ZwCreateFile(&targetFileHandle, FILE_READ_DATA | FILE_WRITE_DATA, &targObjAttr, &IoStaBlock, NULL,
			FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ, FILE_OPEN_IF,
			FILE_NON_DIRECTORY_FILE | FILE_RANDOM_ACCESS | FILE_SYNCHRONOUS_IO_NONALERT, 0, 0);
		if (Status != STATUS_SUCCESS)
		{
			DbgPrint("[mohui] Error:Open target File !\n");
			break;
		}

		// ??仺??????????
		pFileBuufer = ExAllocatePoolWithTag(PagedPool, ulFileBufSize, 'File');
		if (!pFileBuufer)
		{
			DbgPrint("[mohui] Error:ExAllocatePoolWithTag Buffer!\n");
			break;
		}
		int i = 1;
		while (1)
		{
			ulBufLen = ulFileBufSize;
			Status = ZwReadFile(SourceFileHandle, NULL, NULL, NULL, &IoStaBlock, pFileBuufer,
				ulBufLen, &ByOffset, NULL);
			if (!NT_SUCCESS(Status))
			{
				if (Status == STATUS_END_OF_FILE)
				{
					DbgPrint("[mohui] The File Read Complete, Is %d read!\n", i);
					Status = STATUS_SUCCESS;
					break;
				}
			}
			ulBufLen = (ULONG)IoStaBlock.Information;
			Status = ZwWriteFile(targetFileHandle, NULL, NULL, NULL, &IoStaBlock, pFileBuufer,
				ulBufLen, &ByOffset, NULL);
			if (!NT_SUCCESS(Status))
				break;
			ByOffset.QuadPart += ulBufLen;
			i++;
		}

	} while (0);
	if (SourceFileHandle)
	{
		ZwClose(SourceFileHandle);
		SourceFileHandle = NULL;
	}
	if (targetFileHandle)
	{
		ZwClose(targetFileHandle);
		targetFileHandle = NULL;
	}
	if (pFileBuufer)
	{
		ExFreePoolWithTag(pFileBuufer, 'File');
		pFileBuufer = NULL;
	}
	return STATUS_SUCCESS;
}

static KEVENT h_event;			// ???¼??
NTSTATUS DirverThread()
{
	HANDLE ThreadHandle = NULL;		// ??̾?
	UNICODE_STRING usStr = { 0 };	// ??̴???Ĳ??
	RtlInitUnicodeString(&usStr, L"[Message]Hello Thread Kernel");
	KeInitializeEvent(&h_event, SynchronizationEvent, FALSE);		// ??ʼ?????Ϊͬ?????????Ϊ?????̬
	NTSTATUS status = 0;
	do
	{
		status = PsCreateSystemThread(&ThreadHandle, 0, NULL, NULL, NULL, CustomThreadProc, (PVOID)&usStr);
		if (!NT_SUCCESS(status))
		{
			DbgPrint("[Message]:Error Start New Thread fail!\n");
			break;
		}
		DbgPrint("[Message]:Start New Thread OK! Data:%wZ  Address:%p\n", &usStr, &usStr);
	} while (0);

	if (ThreadHandle)
		ZwClose(ThreadHandle);

	// ?????̽?յ?????????̬ʱ???ִ?????
	KeWaitForSingleObject(&h_event, Executive, KernelMode, 0, NULL);
	DbgPrint("[Message]:Function Exit!\n");
	return STATUS_SUCCESS;
}

// ?????̵Ļص????
VOID CustomThreadProc(PVOID context)
{
	PUNICODE_STRING str = (PUNICODE_STRING)context;
	DbgPrint("[Message]:Transef Data:%wZ Address:%p\n:", str, str);
	KeSetEvent(&h_event, 0, TRUE);
	PsTerminateSystemThread(STATUS_SUCCESS);
}

NTSTATUS ControlDevice()
{
	NTSTATUS hStatus = 0;
	UNICODE_STRING  hCDO_Name = RTL_CONSTANT_STRING(usDeviceName);	// ????????
	UNICODE_STRING hCOB_Syb = RTL_CONSTANT_STRING(usSymblName);	// ????????
	hStatus = IoCreateDevice(pDriverObj, 0, &hCDO_Name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN,
		FALSE, &pDeviceObj);  // ӵ?????Ȩ??Ľ??̲??ͨ?

	if (!NT_SUCCESS(hStatus))	// ?????????????
	{
		if (hStatus == STATUS_OBJECT_NAME_COLLISION)	// ???????????
		{
			DbgPrint("[Message]: The [%wZ] Device Object Name repeat!\n", &hCDO_Name);
			return STATUS_OBJECT_NAME_EXISTS;
		}
		else if (hStatus == STATUS_INSUFFICIENT_RESOURCES)	// ?Դ???
		{
			DbgPrint("[Message]: The [%wZ] Device Insufficient resources!\n", &hCDO_Name);
			return STATUS_RESOURCE_REQUIREMENTS_CHANGED;
		}
		DbgPrint("[Message]: The [%wZ] Device Error Unknown!\n", &hCDO_Name);
		return STATUS_UNKNOWN_REVISION;		// ?????
	}
	hStatus = IoCreateSymbolicLink(&hCOB_Syb, &hCDO_Name);
	if (!NT_SUCCESS(hStatus))	//????????ķ?????????
	{
		DbgPrint("[Message]: The [%wZ] Device Failed to establish link!\n", &hCDO_Name);
		IoDeleteDevice(pDeviceObj);
		pDeviceObj = NULL;
		return hStatus;
	}
	// .......?????????ӽ????ɹ?
	DbgPrint("[Message]: The [%wZ] Device Link established successfully,Status OK!\n", &hCDO_Name);
	return STATUS_SUCCESS;
}

// ????ַ????
NTSTATUS cwkDispatch(PDEVICE_OBJECT dev, PIRP irp)
{
	PIO_STACK_LOCATION irpsp = IoGetCurrentIrpStackLocation(irp);	// ?????ǰ???̵Ķ???ռ?
	NTSTATUS status = STATUS_SUCCESS;			// ???ִ?״̬
	ULONG retLen = 0;			// ???ظ?Ӧ???????????

	while (dev == pDeviceObj)	// ????????ɵĿ??豸???
	{
		if (irpsp->MajorFunction == IRP_MJ_CREATE || irpsp->MajorFunction == IRP_MJ_CLOSE)	// ?????Ƿ?Ǵ򿪻?߹ر???
		{
			if (irpsp->MajorFunction == IRP_MJ_CREATE)
				DbgPrint("[Message]: ????ӵ?Ӧ??????????????n");
			if (irpsp->MajorFunction == IRP_MJ_CLOSE)
				DbgPrint("[Message]: ????ӵ?Ӧ????ر??????????n");
			break;
		}

		if (irpsp->MajorFunction == IRP_MJ_DEVICE_CONTROL)	// ????豸?????
		{
			DbgPrint("[Message]: ????ӵ?Ӧ??????????????????n");
			PVOID pBuf = irp->AssociatedIrp.SystemBuffer;		// Ӧ??㷢??Ļ????
			ULONG inLen = irpsp->Parameters.DeviceIoControl.InputBufferLength;	// ??????????
			ULONG outLen = irpsp->Parameters.DeviceIoControl.OutputBufferLength;	// ???????????
			switch (irpsp->Parameters.DeviceIoControl.IoControlCode)
			{
			case CTL_CODE(FILE_DEVICE_UNKNOWN, 0x911, METHOD_BUFFERED, FILE_WRITE_DATA):	// Ӧ??㷢?
			{
				ASSERT(pBuf != NULL);
				ASSERT(inLen > 0);
				ASSERT(outLen == 0);
				DbgPrint("[Message]: ?ɹ???յ?Ӧ????Ϣ -- %s \n", (char*)pBuf);
				break;
			}
			case (ULONG)CTL_CODE(FILE_DEVICE_UNKNOWN, 0x912, METHOD_BUFFERED, FILE_READ_DATA):		// Ӧ????
				DbgPrint("[Message]: Ӧ???????????Ϣ [%wZ]\n", (char*)pBuf);
			default:
				DbgPrint("[Message]: Ӧ????Ϣ???????[%wZ]\n", (char*)pBuf);
				break;
			}
		}
		break;
	}
	irp->IoStatus.Information = retLen;
	irp->IoStatus.Status = status;
	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return status;
}

// ???????ػ???????豸???
PDEVICE_OBJECT KeCreateDevice(PDRIVER_OBJECT pDriObj, PWCHAR strDeviceName)
{
	PDEVICE_OBJECT pDevice = NULL;
	UNICODE_STRING uDeviceName = { 0 };
	do
	{
		if (pDriObj == NULL || strDeviceName == NULL)
			break;

		RtlInitUnicodeString(&uDeviceName, strDeviceName);
		IoCreateDevice(pDriObj, 0, &uDeviceName, FILE_DEVICE_NETWORK, FILE_DEVICE_SECURE_OPEN, TRUE, &pDevice);
		if (!pDevice)
			break;
		DbgPrint("[Message]: ???ػ????豸?????ɳɹ?");
	} while (FALSE);
	return pDevice;
}


// ????ϵͳϵͳ?????ػ?ִ??Ļص????
NTSTATUS SysShutdown(PDEVICE_OBJECT DeviceObject, PIRP pIrp)
{
	DbgPrint("[Message]: ????ϵͳ????????ر");
	UNREFERENCED_PARAMETER(DeviceObject);
	NTSTATUS nStatus = STATUS_SUCCESS;
	HANDLE hDevHand = NULL;
	OBJECT_ATTRIBUTES oObjAttr = { 0 };
	ULONG ulOutK;
	InitializeObjectAttributes(&oObjAttr, RegPath, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);
	/* ???Ϣ????ע??? */

	nStatus = ZwCreateKey(&hDevHand, KEY_READ | KEY_WRITE, &oObjAttr, 0, NULL, REG_OPTION_NON_VOLATILE, &ulOutK);
	if (nStatus != STATUS_SUCCESS)
		return nStatus;

	/* ??????*/
	UNICODE_STRING str = RTL_CONSTANT_STRING(L"SysShut");

	ZwSetValueKey(hDevHand, &str, 0, REG_BINARY, pIrp, sizeof(IRP));
	ZwClose(hDevHand);

	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS ReadShutdownIni()
{
	NTSTATUS nStatus = STATUS_SUCCESS;
	HANDLE hDevHand = NULL;
	OBJECT_ATTRIBUTES oObjAttr = { 0 };
	ULONG ulOutK;
	InitializeObjectAttributes(&oObjAttr, RegPath, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);
	nStatus = ZwCreateKey(&hDevHand, KEY_READ | KEY_WRITE, &oObjAttr, 0, NULL, REG_OPTION_NON_VOLATILE, &ulOutK);
	if (nStatus != STATUS_SUCCESS && hDevHand != 0)
	{
		DbgPrint("[Message]: ?????ʧ??\n");
		return nStatus;
	}
	UNICODE_STRING str = RTL_CONSTANT_STRING(L"SysShut");
	nStatus = ZwQueryValueKey(hDevHand, &str, KeyValuePartialInformation, NULL, 0, &ulOutK);
	if (nStatus == STATUS_BUFFER_TOO_SMALL && ulOutK != 0)
	{
		DbgPrint("[Message]: ע???????ͳ?ػ?ʱ???????\n");
	}
	else
	{
		if (nStatus == STATUS_OBJECT_NAME_NOT_FOUND)
			DbgPrint("[Message]: ע??ֵ?????%u", ulOutK);

		else if (nStatus == STATUS_BUFFER_OVERFLOW)
			DbgPrint("[Message]: ע??????Ļ????̫С?????ֻд???????Ĳ??????\n");

		else if (nStatus == STATUS_INVALID_PARAMETER)
			DbgPrint("[Message]: ע??KeyInformationClass???????Ч??\n");

		else
			DbgPrint("[Message]: ע??δ֪???\n");
	}
	ZwClose(hDevHand);
	hDevHand = NULL;

	return STATUS_SUCCESS;

}