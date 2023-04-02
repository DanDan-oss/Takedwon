#pragma once
#define _CRT_SECURE_NO_WARNING
#include <stdexcept>
#include <time.h>
#include "Draw.h"
#include "Key.h"
#include "Music.h"

#define WIDGET 60
#define HEIGHT 45

#define GAME_WIDGET 40
#define GAME_HEIGHT 45

#define SNAKE 0
#define FEEL 1
#define NONE 2

#define GAME_PAUSE
#define GAME_START
#define GAME_STOP

class Snake
{
public:
	Snake();
	~Snake();

	void snake_init(void);
	void snake_draw(int x, int y, int mode);
	void snake_main(void);

	void print_tile(void);
	void snake_clear(void);
	void snake_fram(void);
	void snake_desc(void);
	void snake_menu(void);

	void point_01(int mode);
	void point_02(int mode);
	void point_03(int mode);
	void point_04(int mode);
	void point_05(int mode);
	void point_06(int mode);

	void snake_two_menu(void);
	void snake_three_menu(void);
	void menu_key_handle(void);

	void fist_game_map(void);
	void game_get_cmd(void);
	void sanke_game_init(void);

	void sanke_feel(void);
	void sanke_detele(void);
	void sanke_add(void);
	void sanke_start(void);
	void sanke_pause(void);

	void game_map_clear(void);
	void game_stop(void);
	void sanke_start_game(void);

	void snake_stop_menu(void);

	void game_run(void);



private:
	Draw draw;

	char map[HEIGHT][WIDGET];
	
	int row;
	int col;

	int main_choose;

	int menu;

	int two_menu_choose;
	int three_mune_choose;

	char key_dir;

	int flush_time_ms;

	int game_status;

	int number;
	int stop_menu_choose;

	char game_map[GAME_HEIGHT][GAME_WIDGET];

	struct snake_node {
		int x;
		int y;
		struct sanke_node* pNnext;

	};

	struct feel_node {
		int x;
		int y;
	};

	struct feel_node feel_postion;

	struct snake_dev{
		struct snake_node* pBody;
		char snake_dir;
		char snake_last_dir;

	};
	struct snake_dev snake;
};

