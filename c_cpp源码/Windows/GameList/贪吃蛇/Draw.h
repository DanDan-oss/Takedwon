#pragma once
#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

#define COLOR_NONE 0xffff
#define COLOR_BLACK 0

class Draw
{
public:
	Draw();
	~Draw();

	void set_xy(int x, int y);
	void set_color(int background, int front);
	void close_cursor(void);
	void set_control_size(int widght, int height);
private:
	int fornt_color;
	int background_color;

};



