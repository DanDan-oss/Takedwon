//#include "function.h"
//void function1(int i)
//{
//	__asm
//	{
//
//		pushad
//
//		push 0x04			// 生成的方式
//		push 0x04			// 生成物品的类型
//		push 0x3c			// 物品出现的X坐标
//		push 0x146			// 物品出现的Y坐标
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
	int unknown_2[4];		// 活力 最大活力  精力  最大精力
	int unknown_3[1];
	int unknown_4[7];
	int unknown_5[3];
	int unknown_6[10];		// 体力 定力 身法 潜能 力量 灵力 外攻 内攻 外防 内防 
	int hpMax;
	int mpMax;
	int unknown_7[2];
	int unknown_8[4];		// 命中 闪避 会攻 会防

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
		printf("获取游戏pid失败！\n");
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
		printf("ERROR: 获取游戏Pid错误!\n");
		return;
	}
	HANDLE hGame = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hGame)
	{
		int typeH;
		LPVOID myData = VirtualAllocEx(hGame, NULL, 100, MEM_COMMIT, PAGE_NOACCESS);		// 跨线程创建空间
		WriteProcessMemory(hGame, (LPVOID)myData, &function1, sizeof(void*), NULL);			// 代码注入
		printf("游戏注入成功PID:%d\n", pid);
		while (1)
		{
			printf("请输入要增加数据的类型:");
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
