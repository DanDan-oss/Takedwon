#pragma once
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <map>
#include <TlHelp32.h>


#define GAME_NAME L"PlantsVsZombies.exe"
#define DEBUG



void function1(int);					// ��������call
BOOL traverseProcesses(int*);				// ͨ�������ȡ��ϷPID
void function2(int pid);