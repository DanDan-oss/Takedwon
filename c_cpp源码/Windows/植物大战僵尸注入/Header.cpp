//#include "function.h"
//void function1(int i)
//{
//	__asm
//	{
//
//		pushad
//
//		push 0x04			// ���ɵķ�ʽ
//		push 0x04			// ������Ʒ������
//		push 0x3c			// ��Ʒ���ֵ�X����
//		push 0x146			// ��Ʒ���ֵ�Y����
//		mov eax, DWORD PTR DS : [0x006A9EC0]
//		mov ecx, DWORD PTR DS : [eax + 0x768]
//		mov eax, 0x0040cb10
//		call eax
//
//		popad
//
//		/*
//	pushad
//	push 0x04
//	push 0x8
//	push 0x3c
//	push 0x105
//	mov ecx, 0x1e8006f8
//	mov eax, 0x0040ff90
//	call eax
//	popad
//	*/
//
//	}
//}
//

struct Role {
	int Hp;
	int Mp;
	int Exp;
	int unknown_1[2];
	int unknown_2[4];		// ���� ������  ����  �����
	int unknown_3[1];
	int unknown_4[7];
	int unknown_5[3];
	int unknown_6[10];		// ���� ���� �� Ǳ�� ���� ���� �⹥ �ڹ� ��� �ڷ� 
	int hpMax;
	int mpMax;
	int unknown_7[2];
	int unknown_8[4];		// ���� ���� �ṥ ���

};
bool traverseprocesses(int* pid)
{

	processentry32 pe32;
	pe32.dwsize = sizeof(pe32);
	handle hprocesssnap = createtoolhelp32snapshot(th32cs_snapprocess, 0); // ???
	if (hprocesssnap == invalid_handle_value)
	{
		printf("error: createtoolhelp32snapshot!\n");
		return 0;
	}
	bool bresult = process32first(hprocesssnap, &pe32);
	while (bresult)
	{
		if (!lstrcmpi(pe32.szexefile, game_name))
		{
			*pid = pe32.th32processid;
		}
		bresult = process32next(hprocesssnap, &pe32);
	}
	if (*pid == 0)
	{
		printf("��ȡ��Ϸpidʧ�ܣ�\n");
		closehandle(hprocesssnap);
		return 0;
	}
	return 1;
}

void function2(int pid)
{
	int pid = 0;
	if (!traverseprocesses(&pid))
	{
		printf("ERROR: ��ȡ��ϷPid����!\n");
		return;
	}
	HANDLE hGame = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hGame)
	{
		int typeH;
		LPVOID myData = VirtualAllocEx(hGame, NULL, 100, MEM_COMMIT, PAGE_NOACCESS);		// ���̴߳����ռ�
		WriteProcessMemory(hGame, (LPVOID)myData, &function1, sizeof(void*), NULL);			// ����ע��
		printf("��Ϸע��ɹ�PID:%d\n", pid);
		while (1)
		{
			printf("������Ҫ�������ݵ�����:");
			scanf_s("%d", &typeH);
			CreateRemoteThread(hGame, NULL, 0, (LPTHREAD_START_ROUTINE)&function1, NULL, 0, NULL);

			//}
		}
	}
	else {
		printf("error: OpenProcess !\n");
#ifdef DEBUG
		printf("%d\n", GetLastError());
#endif // DEBUG

	}
	CloseHandle(hGame);
}
