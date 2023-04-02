#include "pch.h"
#include "GameHunter.h"

GameHunter::GameHunter()
{
	if (GetGamePid() == GET_PID_ERROR)
	{
		InError = GET_PID_ERROR;
		return;
	}
	hGameProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwGamePid);
	if (!hGameProcess)
	{
		MessageBox(NULL, TEXT("打开游戏进程失败!"), TEXT("错误"), MB_OK);
		InError = OPEN_PROCESS_ERROR;
		return;
	}
	pCodeInjectionAddress = VirtualAllocEx(hGameProcess, NULL, 0x200, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!pCodeInjectionAddress)
	{
		MessageBox(NULL, TEXT("跨进程创建空间失败"), TEXT("错误"), MB_OK);
		InError = VIRTUAL_ALLOC_EX_ERROR;
		return;
	}
	BYTE Hack_Call[] = {
		0x8b, 0x50, 0x2c,			// jnp [esp+002c50]
		0x8b, 0x40, 0x28,			// jnp [esp+002840]
		0x52,						// push edx
		0x6A,0x14,
		0x8b, 0x44, 0x24, 0x20,
		0x56,						// push esi
		0x55,						// push ebp
		0xe9, 0x00, 0x00, 0x00, 0x00,
	};
	DWORD dwOffset = 0x413fbd - ((DWORD)pCodeInjectionAddress + sizeof(Hack_Call));
	memcpy((LPVOID)((DWORD)Hack_Call + sizeof(Hack_Call) - 4), &dwOffset, 4);
	WriteProcessMemory(hGameProcess, pCodeInjectionAddress, Hack_Call, sizeof(Hack_Call), NULL);

	BYTE ModifyCode[] =
	{
		0xe9, 0x00, 0x00, 0x00, 0x00,
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
	};
	pModifyCode = ModifyCode;

	dwOffset = (DWORD)pCodeInjectionAddress - (0x413faf + 5);
	memcpy((LPVOID)((DWORD)ModifyCode + 1), &dwOffset, 4);

	BYTE RepairCode[] =
	{
		0x8b, 0x50, 0x2c,
		0x8b, 0x40, 0x28,
		0x52, 0x50,
		0x8b, 0x44, 0x24, 0x20,
		0x56, 0x55
	};
	pRepairCode = RepairCode;
	
}

GameHunter::~GameHunter()
{
	if (!hGameProcess)
	{
		CloseHandle(hGameProcess);
	}
}

DWORD GameHunter::GetGamePid()
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // ???
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, TEXT("打开任务管理器 HANDLE 失败"), TEXT("错误"), MB_OK);
		return  GET_PID_ERROR;
	}
	BOOL bResult = Process32First(hProcessSnap, &pe32);
	while (bResult)
	{
		if (!lstrcmpi(pe32.szExeFile, lpGameName))
		{
			this->dwGamePid = pe32.th32ProcessID;
			break;
		}
		bResult = Process32Next(hProcessSnap, &pe32);
	}
	if (dwGamePid == 0)
	{
		MessageBox(NULL, TEXT("获取游戏PID失败!"), TEXT("PID"), MB_OK);
		CloseHandle(hProcessSnap);
		return  GET_PID_ERROR;
	}
	CloseHandle(hProcessSnap);
	return GET_PID_OK;
}

