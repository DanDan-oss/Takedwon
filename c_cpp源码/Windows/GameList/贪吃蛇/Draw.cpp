#define _CRT_SECURE_NO_WARNINGS
#include "Draw.h"

Draw::Draw()
{
	this->background_color = BACKGROUND_INTENSITY;
	this->fornt_color = FOREGROUND_INTENSITY;
}

Draw::~Draw()
{

}

void Draw::set_xy(int x, int y)
{
	COORD postion;

	postion.X = x * 2;
	postion.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), postion);
}

void Draw::set_color(int background, int front)
{
	if (background != COLOR_NONE) {
		this->background_color = background;
	}
	if (front != COLOR_NONE) {
		this->fornt_color = front;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), this->background_color | this->fornt_color);
}

void Draw::close_cursor(void)
{
	CONSOLE_CURSOR_INFO cursor_info;
	cursor_info = { 1,0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void Draw::set_control_size(int widght, int height)
{
	char set[128];
	memset(set, 0x00, 128);
	widght *= 2;
	sprintf(set, "mode con cols=%d lines=%d", widght, height);
	system(set);
}

