#include <stdio.h>
#include <iostream>
#include "function.h"
/*
void function1_1()
{

	__asm
	{

		pushad
		push 0x04			// ���ɵķ�ʽ
		push 0x04			// ������Ʒ������
		push 0x3c			// ��Ʒ���ֵ�X����
		push 0x146			// ��Ʒ���ֵ�Y����
		mov eax, 0x0040cb10
		call eax
		popad


	pushad
	push 0x04
	push 0x8
	push 0x3c
	push 0x105
	mov ecx, 0x1e8006f8
	mov eax, 0x0040ff90
	call eax
	popad


	}
}
*/
int main()
{
	int error;
	int a = 0;
	int* pid = &a;
	if ((error = traverseProcesses(pid)) == 0)
	{
		system("pause");
		return 0;
	}

	function2(*pid);
	/*
HWND HW = FindWindow(NULL, L"Plants vs. Zombies");
DWORD PID{};
GetWindowThreadProcessId(HW, &PID);
HANDLE HProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
LPCVOID HACK = VirtualAllocEx(HProcess, NULL, 0x100, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
std::cout << HACK;
WriteProcessMemory(HProcess, (LPVOID)HACK, &function1_1, 0x100, NULL);
system("pause");
CreateRemoteThread(HProcess, NULL, 0, (LPTHREAD_START_ROUTINE)&HACK, NULL, 0, NULL);
*/
	system("pause");
}