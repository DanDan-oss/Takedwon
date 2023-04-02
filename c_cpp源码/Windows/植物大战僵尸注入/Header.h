#pragma once
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <map>
#include <TlHelp32.h>


#define GAME_NAME L"PlantsVsZombies.exe"
#define DEBUG



void function1(int);					// 阳光生成call
BOOL traverseProcesses(int*);				// 通过句柄获取游戏PID
void function2(int pid);