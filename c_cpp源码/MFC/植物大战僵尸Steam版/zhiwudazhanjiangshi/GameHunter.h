#pragma once
#include <windows.h>
#include <TlHelp32.h>

#define GET_PID_ERROR -1
#define GET_PID_OK 1

#define OPEN_PROCESS_ERROR -2
#define VIRTUAL_ALLOC_EX_ERROR -3

class GameHunter
{
public:
	GameHunter();
	~GameHunter();
	INT InError = 1;
	INT pCodeState = 0;

	LPVOID pModifyCode;
	LPVOID pRepairCode;
	HANDLE hGameProcess;
private:
	LPSTR lpGameName = "popcapgame1.exe";
	DWORD dwGamePid = 0;


	LPVOID pCodeInjectionAddress;
	


	DWORD GetGamePid();
	DWORD InjectionCode();
};

