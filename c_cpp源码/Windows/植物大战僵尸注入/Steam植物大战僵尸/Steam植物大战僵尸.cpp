#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <map>
#include <TlHelp32.h>

#define _CRT_SECURE_NO_WARNINGS

#define GAME_NAME TEXT("popcapgame1.exe")

BOOL traverseProcesses(int* pid);


//void SUN()
//{
//	__asm
//	{
//		pushad
//		push 0x04			// 生成的方式
//		push 0x04			// 生成物品的类型
//		push 0x3c			// 物品出现的X坐标
//		push 0x146			// 物品出现的Y坐标
//		mov eax, DWORD PTR DS : [0x0400000 + 0x006A9EC0]
//		mov ecx, DWORD PTR DS : [eax + 0x768]
//		mov eax, 0x0040cb10
//		call eax
//		popad
//	}
//}

void	MAP()
{
	__asm
	{
		pushad

		mov edx, 0xffffffff
		mov eax, 0x2

		push edx
		push eax						// type
		mov eax, 0x02					// X
		push 0x2						// Y		

		mov eax, dword ptr ds : [0x0400000 + 0x0331cdc]
		mov eax, dword ptr ds:[eax + 0x0868]		// popcapgame1.exe+331CDC +  00000868
		push eax

		mov eax, 0x02					// X

		mov ecx, 0x004105a0
		call ecx

		popad
	
	}
	return;

}





int injectionFunction_1()
{

	BYTE hack[] = {
		0xe9, 0x00, 0x00, 0x00, 0x00,
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
	};

	BYTE Getcall[] = {

		0x8b, 0x50, 0x2c,
		0x8b, 0x40, 0x28,
		0x52,
		0x6A,0x14,
		0x8b, 0x44, 0x24, 0x20,
		0x56,
		0x55,
		0xe9, 0x00, 0x00, 0x00, 0x00,
	};
	int pid = 0;
	if (!traverseProcesses(&pid))
	{
		printf("ERROR: Failed to Get Game PID!\n");
		return NULL;
	}
	HANDLE hGameProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hGameProcess)
	{
		printf("ERROR: Failed to open Process!\n");
		return NULL;
	}
	LPVOID lpGameAddress = VirtualAllocEx(hGameProcess, NULL, 0x150, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!lpGameAddress)
	{
		printf("ERROR: Cross process space request failed\n");
		CloseHandle(hGameProcess);
		return NULL;
	}
	
	DWORD dwOffset = (DWORD)lpGameAddress - (0x413faf + 5);
	memcpy((LPVOID)((DWORD)hack + 1), &dwOffset, 4);
	printf("  %p   %.6x    ", lpGameAddress, dwOffset);

	dwOffset = 0x413fbd - ((DWORD)lpGameAddress + sizeof(Getcall));
	memcpy((LPVOID)((DWORD)Getcall + sizeof(Getcall) - 4), &dwOffset, 4);
	printf("%.6x\n",dwOffset);
	if (!WriteProcessMemory(hGameProcess, (LPVOID)0x413faf, hack, sizeof(hack), NULL))
	{
		printf("ERROR: hack injection failed!\n");
		CloseHandle(hGameProcess);
		return NULL;
	}
	DWORD dwProtect = 0;
	if (!VirtualProtectEx(hGameProcess, lpGameAddress, sizeof(Getcall)+1, PAGE_EXECUTE_READWRITE, &dwProtect))
	{
		printf("ERROR: Modify address permission!\n");
		CloseHandle(hGameProcess);
		return NULL;
	}

	if (!WriteProcessMemory(hGameProcess, lpGameAddress, Getcall, sizeof(Getcall), NULL))
	{
		printf("ERROR: Getcall injection failed!\n");
		CloseHandle(hGameProcess);
		return NULL;
	}

	CloseHandle(hGameProcess);
	printf("Start OK!\n");
	return TRUE;
}

int zhuru()
{
	BYTE arr[] = {
		0x60,
		0xBA, 0xFF, 0xFF, 0xFF, 0xFF,
		0xB8, 0x02, 0x00, 0x00, 0x00,
		0x52,
		0x50,
		0xB8, 0x02, 0x00, 0x00, 0x00,
		0x6A, 0x02,
		0x3E, 0xA1, 0xDC, 0x1C, 0x73, 0x00,
		0x3E, 0x8B, 0x80, 0x68, 0x08, 0x00, 0x00,
		0x50,
		0xB8, 0x02, 0x00, 0x00, 0x00,
		0xB9, 0xA0, 0x05, 0x41, 0x00,
		0xFF, 0xD1,
		0x61,
		0xe8,
		0xc2, 0x0c, 0x00
	};
	int pid =0;
	if(!traverseProcesses(&pid))
	{
		printf("error: traverseProcesses !\n");
		system("pause");
		return NULL;
	}
	printf("Pid : %d\n", pid);
	HANDLE hGame = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hGame)
	{
		LPVOID myData = VirtualAllocEx(hGame, NULL, 0x4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);		// 跨线程创建空间
		if (!myData)
		{
			printf("error: VirtualAllocEx !\n");
			return NULL;
		}
		DWORD dwInfo = 0;
		if (!VirtualProtectEx(hGame, myData, 0x1000, PAGE_EXECUTE_READWRITE, &dwInfo))
		{
			printf("ERROR: 修改内存保护属性失败!\n");
		}
		if (!WriteProcessMemory(hGame, myData, &MAP, 0x1000, NULL))			// 代码注入
		{
			printf("游戏注入失败PID:%d\n", pid);
			return NULL;
		}
		printf("游戏注入成功PID:%d   %p\n", pid, myData);
		int i = 1;
		while (i)
		{
			scanf_s("%d", &i);
			CreateRemoteThread(hGame, NULL, 0, (LPTHREAD_START_ROUTINE)myData, NULL, 0, NULL);
		}
	}
	else {
		printf("error: OpenProcess !\n");
#ifdef DEBUG
		printf("%d\n", GetLastError());
#endif // DEBUG

	}
	CloseHandle(hGame);
	system("pause");
}

BOOL traverseProcesses(int* pid)
{

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // ???
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf("error: CreateToolhelp32Snapshot!\n");
		return 0;
	}
	BOOL bResult = Process32First(hProcessSnap, &pe32);
	while (bResult)
	{
		if (!lstrcmpi(pe32.szExeFile, GAME_NAME))
		{
			*pid = pe32.th32ProcessID;
			break;
		}
		bResult = Process32Next(hProcessSnap, &pe32);
	}
	if (*pid == 0)
	{
		printf("获取游戏PID失败！\n");
		CloseHandle(hProcessSnap);
		return 0;
	}
	CloseHandle(hProcessSnap);
	return 1;
}

int main()
{
	injectionFunction_1();
	system("pause");
	return NULL;
}