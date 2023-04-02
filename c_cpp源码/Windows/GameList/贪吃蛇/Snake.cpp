#include "Snake.h"
#define DESC_OFFSET 3
#define TWO_OFFSET 2

Snake::Snake()
{
	this->main_choose = 2;
	this->menu = 1;
	this->key_dir = ' ';
	this->two_menu_choose = 1;
	this->three_mune_choose = 0;
	this->flush_time_ms = 500;
	this->stop_menu_choose = 0;
}

Snake::~Snake()
{

}

void Snake::snake_init(void)
{
	this->draw.close_cursor();
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDGET; j++) {
			this->map[i][j] = 0;
		}
	}

	this->row = GAME_WIDGET;
	this->col = GAME_HEIGHT;

	this->draw.set_control_size(WIDGET, HEIGHT);
	system("贪吃蛇");
}

void Snake::snake_draw(int x, int y, int mode)
{
	this->draw.set_xy(x, y);
	if (mode == SNAKE) {
		cout << "■";
	}
	else if (mode == FEEL)
	{
		cout << "⊙";
	}
	else {
		cout << "  ";
	}

}

void Snake::snake_main(void)
{
	snake_clear();
	snake_fram();
	print_tile();
	snake_menu();
	snake_desc();
	while (1) {
		if (key_get(this->key_dir)) {
			try {
				menu_key_handle();
			}
			catch (...) {
				break;
			}
		}
	}
}

void Snake::print_tile(void)
{
	int row_tmp = 14;
	int col_tmp = 3;

	this->draw.set_color(COLOR_BLACK, FOREGROUND_RED);
	this->draw.set_xy(row_tmp, col_tmp++);
	cout << "    __________       ___";												this->draw.set_xy(row_tmp, col_tmp++);
	cout << "   /          \\     / \\ \\    |____      __\\__";					this->draw.set_xy(row_tmp, col_tmp++);
	cout << "  /  ________  \\   / ___ \\  _/ __     | |   /  ";					this->draw.set_xy(row_tmp, col_tmp++);
	cout << "  |  |      |__|     _/_   |_|  /    [|] |/      ";					this->draw.set_xy(row_tmp, col_tmp++);
	cout << "  |  |              | | |      /     _|_ \\__/";						this->draw.set_xy(row_tmp, col_tmp++);
	cout << "  \\  \\_______        / \\      |___/        ____";					this->draw.set_xy(row_tmp, col_tmp++);
	cout << "   \\         \\    ____ ____      ____   __ |  |  ___   ______";		this->draw.set_xy(row_tmp, col_tmp++);
	cout << "    \\_______  \\   |  |/    \\    /    \\_/ / |  | /  /  /      \\";  this->draw.set_xy(row_tmp, col_tmp++);
	cout << "            \\  \\  |    ___  \\  / ____   /  |  |/  /  /  ____  \\";	this->draw.set_xy(row_tmp, col_tmp++);
	cout << "  __        |  |  |   /   \\  \\ | |  |  /   |     /  |  /____\\  |";	this->draw.set_xy(row_tmp, col_tmp++);
	cout << " \\  \\_______|  |  |  |    |  | | |__|  |   |     \\  |  ________/";	this->draw.set_xy(row_tmp, col_tmp++);
	cout << "  \\            /  |  |    |  |  \\       \\  |  |\\  \\  \\  \\____";	this->draw.set_xy(row_tmp, col_tmp++);
	cout << "   \\__________/   |__|    |__|   \\___/\\__\\ |__| \\__\\  \\______/";
}

void Snake::snake_clear(void)
{
	this->draw.set_color(COLOR_BLACK, COLOR_BLACK);
	for (int i = 1; i < HEIGHT - 1; i++) {
		for (int j = 1;j < WIDGET - 1;j++) {
			this->draw.set_xy(j, i);
			cout << "  ";
			this->map[i][j] = 0;
		}
	}
}

void Snake::snake_fram(void)
{
	this->draw.set_color(BACKGROUND_BLUE, FOREGROUND_BLUE);

	for(int i=0; i<WIDGET; i++){
		this->draw.set_xy(i, 0);
		cout << "■";
		this->draw.set_xy(i, HEIGHT - 1);
		cout << "■";
	}
	for (int i = 0; i < HEIGHT;i++) {
		this->draw.set_xy(0, i);
		cout << "■";
		this->draw.set_xy(WIDGET - 1, i);
		cout << "■";
	}
	this->draw.set_xy(1, 1);
}

void Snake::snake_desc(void)
{
	this->draw.set_color(COLOR_BLACK, FOREGROUND_INTENSITY);
	int temp_row = WIDGET / 2 - DESC_OFFSET;
	int temp_col = HEIGHT / 3 + 15;
	this->draw.set_xy(temp_row, temp_col);
	cout << "游戏说明";
	this->draw.set_xy(temp_row - 5, temp_col + 2);
	cout << "←↑↓→ 控制分别对应 a w s d";
	this->draw.set_xy(temp_row - 5, temp_col + 4);
	cout << "红色代表选中,蓝色代表未选中";
	this->draw.set_xy(temp_row - 5, temp_col + 6);
	cout << "ESC代表退出，Enter代表进入";

}

void Snake::snake_menu(void)
{
	int one_color{ 0 }, two_color{ 0 }, three_color{ 0 }, four_color{ 0 };

	if (this->main_choose == 0) {
		one_color = FOREGROUND_RED;
		two_color = FOREGROUND_BLUE;
		three_color = FOREGROUND_BLUE;
		four_color = FOREGROUND_BLUE;
	} else if (this->main_choose == 1) {
		one_color = FOREGROUND_BLUE;
		two_color = FOREGROUND_RED;
		three_color = FOREGROUND_BLUE;
		four_color = FOREGROUND_BLUE;
	}
	else if (this->main_choose == 2) {
		one_color = FOREGROUND_BLUE;
		two_color = FOREGROUND_BLUE;
		three_color = FOREGROUND_RED;
		four_color = FOREGROUND_BLUE;
	}
	else if (this->main_choose == 3) {
		one_color = FOREGROUND_BLUE;
		two_color = FOREGROUND_BLUE;
		three_color = FOREGROUND_BLUE;
		four_color = FOREGROUND_RED;
	}

	int temp_row = WIDGET / 2 - DESC_OFFSET;
	int temp_col = HEIGHT / 3 + 5;
	
	this->draw.set_color(COLOR_BLACK, one_color);
	this->draw.set_xy(temp_row, temp_col);
	cout << "游戏关卡";
	this->draw.set_color(COLOR_BLACK, two_color);
	this->draw.set_xy(temp_row, temp_col + 2);
	cout << "游戏模式";
	this->draw.set_color(COLOR_BLACK, three_color);
	this->draw.set_xy(temp_row, temp_col + 4);
	cout << "开始游戏";
	this->draw.set_color(COLOR_BLACK, four_color);
	this->draw.set_xy(temp_row, temp_col + 6);
	cout << "退出游戏";
	





}

void Snake::point_01(int mode)
{
	if (mode == 0) {
		this->draw.set_color(COLOR_BLACK, FOREGROUND_INTENSITY);
	}
	else {
		this->draw.set_color(COLOR_BLACK, FOREGROUND_BLUE);
	}
	snake_draw(17 + TWO_OFFSET, 15, SNAKE);
	this->draw.set_xy(16 + TWO_OFFSET, 17);
	cout << "关卡一";
}

void Snake::point_02(int mode)
{
	if (mode == 0) {
		this->draw.set_color(COLOR_BLACK, FOREGROUND_INTENSITY);
	}
	else {
		this->draw.set_color(COLOR_BLACK, FOREGROUND_BLUE);
	}
	snake_draw(26 + TWO_OFFSET, 15, SNAKE);
	snake_draw(28 + TWO_OFFSET, 15, SNAKE);
	this->draw.set_xy(26 + TWO_OFFSET, 17);
	cout << "关卡二";
}

void Snake::point_03(int mode)
{
	if (mode == 0) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	}
	snake_draw(36 + TWO_OFFSET, 15, SNAKE);
	snake_draw(37 + TWO_OFFSET, 15, SNAKE);
	this->draw.set_xy(36 + TWO_OFFSET, 17);
	cout << "关卡三";
}

void Snake::point_04(int mode)
{
	if (mode == 0) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	}
	snake_draw(16 + TWO_OFFSET, 22, SNAKE);
	snake_draw(18 + TWO_OFFSET, 22, SNAKE);
	snake_draw(16 + TWO_OFFSET, 23, SNAKE);
	snake_draw(18 + TWO_OFFSET, 23, SNAKE);
	this->draw.set_xy(16 + TWO_OFFSET, 24);
	cout << "关卡四";
}

void Snake::point_05(int mode)
{
	if (mode == 0) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	}
	snake_draw(26 + TWO_OFFSET, 22, SNAKE);
	snake_draw(28 + TWO_OFFSET, 22, SNAKE);
	snake_draw(26 + TWO_OFFSET, 23, SNAKE);
	snake_draw(27 + TWO_OFFSET, 23, SNAKE);
	snake_draw(28 + TWO_OFFSET, 23, SNAKE);
	this->draw.set_xy(26 + TWO_OFFSET, 24);
	cout << "关卡五";
}

void Snake::point_06(int mode)
{
	if (mode == 0) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	}
	snake_draw(36 + TWO_OFFSET, 22, SNAKE);
	snake_draw(37 + TWO_OFFSET, 22, SNAKE);
	snake_draw(38 + TWO_OFFSET, 22, SNAKE);
	snake_draw(36 + TWO_OFFSET, 23, SNAKE);
	snake_draw(37 + TWO_OFFSET, 23, SNAKE);
	snake_draw(38 + TWO_OFFSET, 23, SNAKE);
	this->draw.set_xy(36 + TWO_OFFSET, 24);
	cout << "关卡六";
}

void Snake::snake_two_menu(void)
{
	if (this->two_menu_choose == 1) { point_01(1); }
	else { point_01(0); }


	if (this->two_menu_choose == 2) { point_02(1); }
	else { point_02(0); }

	if (this->two_menu_choose == 3) { point_03(1); }
	else { point_03(0); }

	if (this->two_menu_choose == 4) { point_04(1); }
	else { point_04(0); }

	if (this->two_menu_choose == 5) { point_05(1); }
	else { point_05(0); }

	if (this->two_menu_choose == 6) { point_06(1); }
	else { point_06(0); }
}

void Snake::snake_three_menu(void)
{
	int one_color{ 0 }, two_color{ 0 };

	if (this->three_mune_choose == 0) {
		one_color = FOREGROUND_RED;
		two_color = FOREGROUND_BLUE;
	}
	else if (this->three_mune_choose == 1) {
		one_color = FOREGROUND_BLUE;
		two_color = FOREGROUND_RED;
	}

	int temp_row = WIDGET / 2 - DESC_OFFSET;
	int temp_col = HEIGHT / 3 + 5;
	this->draw.set_color(COLOR_BLACK, one_color);
	this->draw.set_xy(temp_row, temp_col);
	cout << "正常模式";
	this->draw.set_color(COLOR_BLACK, two_color);
	this->draw.set_xy(temp_row, temp_col + 2);
	cout << "智能模式";
}

void Snake::menu_key_handle(void)
{
	switch (this->key_dir)
	{
	case 'w':
		if (menu == 1) {
			this->main_choose--;
			if (this->main_choose < 0) {
				this->main_choose = 3;
			}
			this->main_choose %= 4;
		}
		else if (menu == 3) {
			this->main_choose--;
			if (this->main_choose < 0) {
				this->main_choose = 1;
			}
			this->main_choose %= 2;
			break;
		}
	case 's':
		if (menu == 1) {
			this->main_choose++;
			this->main_choose %= 4;
		}
		else if (menu == 3) {
			this->main_choose++;
			this->main_choose %= 2;
			break;
		}
	case 'a':
		this->two_menu_choose--;
		if (two_menu_choose < 1) {
			two_menu_choose = 1;
		}
		break;
	case 'd':
		this->two_menu_choose++;
		if (this->two_menu_choose < 6) {
			this->two_menu_choose = 6;
		}
		break;

	default:
		break;

		if ((this->key_dir == 'w') || (this->key_dir == 's')) {
			if (menu == 1) { snake_menu(); }
			if (menu == 3) { snake_three_menu(); }
		}
		else if ((this->key_dir == 'a') || (this->key_dir == 'd')) {
			if (menu == 2) { snake_two_menu(); }
		}
		else if (this->key_dir == 13) {
			if (menu != 1) {
				snake_clear();
				snake_fram();
				print_tile();
				snake_menu();
				snake_desc();
				menu = 1;
			}
			else {
				snake_clear();
				menu = (this->main_choose % 4) + 2;
				if (menu == 2) {
					snake_two_menu();
				}
				else if (menu == 3) {
					snake_three_menu();
				}
				else if (menu == 4) {
					throw"over_main";
				}
				else if (menu == 5) {
					exit(0);
				}
			}
		}
	}
}

void Snake::fist_game_map(void)
{
	this->flush_time_ms = 200;

}

void Snake::game_get_cmd(void)
{
	char rec_c;
	int cout = this->flush_time_ms / 10;
	if (cout <= 1) {
		cout = 2;
	}
	if (this->three_mune_choose == 0) {
		while (cout--) {
			if (key_get(rec_c)) {
				dir_test(rec_c, this->snake.snake_dir);
				break;
			}
		}
	}
	else
	{
		Sleep(this->flush_time_ms);
	}
}

void Snake::sanke_game_init(void)
{
	this->draw.set_color(BACKGROUND_BLUE, FOREGROUND_BLUE);
	for (int i = GAME_WIDGET - 1;i < WIDGET;i++) {
		this->draw.set_xy(i, 22);
		cout << "■";
	}
	for (int i = 0;i < HEIGHT;i++) {
		this->draw.set_xy(GAME_WIDGET - 1, i);
		cout << "■";
	}
	int row_temp = WIDGET - GAME_WIDGET;
	int col_temp = 7;
	row_temp /= 2;
	row_temp -= 4;
	this->draw.set_color(COLOR_BLACK, FOREGROUND_RED);

	this->draw.set_xy(GAME_WIDGET + row_temp, col_temp);
	cout << "当前关卡:" << this->two_menu_choose;
	this->draw.set_xy(GAME_WIDGET + row_temp, col_temp + 2);

	cout << "当前游戏分数:0";
	this->draw.set_xy(GAME_WIDGET + row_temp, col_temp + 4);

	if (this->three_mune_choose == 0) {
		cout << "当前游戏模式：正常";
	}
	else {
		cout << "当前游戏模式智能：";
	}

	int col_temp1 = col_temp + 18;
	int row_temp1 = row_temp;

	this->draw.set_xy(GAME_WIDGET + row_temp1, col_temp1);
	cout << "游戏操作说明：";
	this->draw.set_xy(GAME_WIDGET + row_temp1, col_temp1 + 2);
	cout << "w:上        s:下";
	this->draw.set_xy(GAME_WIDGET + row_temp1, col_temp1 + 4);
	cout << "a:左        d:右";
	this->draw.set_xy(GAME_WIDGET + row_temp1, col_temp1 + 6);
	cout << "  ESC:退出";
	this->draw.set_xy(GAME_WIDGET + row_temp1, col_temp1 + 8);
	cout << "  Engter:退出";
	this->draw.set_xy(GAME_WIDGET + row_temp1, col_temp1 + 10);
	cout << "  空格:暂停";
	this->draw.set_xy(GAME_WIDGET + row_temp1, col_temp1 + 12);
	cout << "  作者：黑兔档案";
	this->draw.set_xy(GAME_WIDGET + row_temp1, col_temp1 + 14);
	cout << "  版本：1.0";

	for (int i = 0;i < GAME_WIDGET;i++) {
		this->game_map[0][i] = 1;
		this->game_map[HEIGHT - 1][i] = 1;

	}

	for (int i = 0;i < GAME_HEIGHT;i++) {
		this->game_map[i][0] = 1;
		this->game_map[i][GAME_WIDGET - 1] = 1;
	}

	switch (this->two_menu_choose)
	{
	case 1:
		fist_game_map();
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	default:
		break;
	}

	snake.pBody = (snake_node*)new snake_node;
	snake.snake_dir = 'a';
	snake.snake_last_dir = 'a';

	snake.pBody->x = 30;
	snake.pBody->y = 30;

	game_map[snake.pBody->y][snake.pBody->x] = 1;
	snake.pBody = (snake_node*)new snake_node;

	snake.pBody->x = 31;
	snake.pBody->y = 30;

	game_map[snake.pBody->y][snake.pBody->x] = 1;

	snake.pBody->pNnext = NULL;

	feel_postion.x = 30;
	feel_postion.y = 23;

	draw.set_color(COLOR_BLACK, FOREGROUND_RED);
	snake_draw(feel_postion.x, feel_postion.y, FEEL);

	this->number = 0;
}

void Snake::sanke_feel(void)
{
	int feel_cout = 0;
	for (int i = 0;i < GAME_HEIGHT - 1;i++) {
		for (int j = 0;j < GAME_WIDGET - 1;j++) {
			if (this->game_map[i][j] == 0) {
				feel_cout++;
			}
		}
	}
	if (feel_cout < 10) {
		throw out_of_range("恭喜你，你赢了");
	}
	else {
		srand((unsigned int)time(NULL));
		int feel_num = 0;
		feel_num = rand() % feel_cout;
		feel_cout = 0;

		for (int i = 1;i < GAME_HEIGHT - 1;i++) {
			for (int j = 0;j < GAME_WIDGET - 1;j++) {
				if (this->game_map[i][j] == 0) {
					feel_cout++;
					if (feel_cout == feel_num) {
						feel_postion.x = j;
						feel_postion.y = i;
					}
				}
			}
		}
		draw.set_color(COLOR_BLACK, FOREGROUND_RED);
		snake_draw(feel_postion.x, feel_postion.y, FEEL);
	}
}

void Snake::sanke_detele(void)
{
	snake_node* ptemp1 = NULL;
	snake_node* ptemp2 = NULL;
	ptemp1 = snake.pBody;
	if (ptemp1 == NULL) {
		return;
	}
	while (ptemp1->pNnext != NULL) {
		ptemp2->pNnext = ptemp1->pNnext;
		delete ptemp1;
		ptemp1 = ptemp2;
	}

	delete ptemp1;
	snake.pBody = NULL;
}

void Snake::sanke_add(void)
{
	struct snake_node* pstruct = snake.pBody;

	while (pstruct->pNnext != NULL) {
		pstruct->pNnext = pstruct->pNnext;
	}
	pstruct = (snake_node*)new snake_node;
	pstruct->pNnext = NULL;
}

void Snake::sanke_start(void)
{
	int x_temp;
	int y_temp;
	snake_node* ptemp = snake.pBody;
	x_temp = ptemp->x;
	y_temp = ptemp->y;

	switch (snake.snake_dir)
	{
	case 'w':
		ptemp->y -= 1;
		break;
	case 's':
		ptemp->y += 1;
		break;
	case 'a':
		ptemp->x -= 1;
		break;
	case 'd':
		ptemp->x += 1;
		break;
	case 32:
		snake.snake_dir = snake.snake_last_dir;
		return;
	case 27:
		end_game_music();
		Sleep(2500);
		play_game_music;
		this->menu = 1;
		throw out_of_range("强制退出");
		break;
	default:
		snake.snake_dir = snake.snake_last_dir;
		break;
	}
	snake.snake_last_dir = snake.snake_dir;

	if (game_map[ptemp->x][ptemp->y] == 1) {
		game_map_clear();
		sanke_detele();
		snake_stop_menu();
		this->draw.set_color(COLOR_BLACK, FOREGROUND_GREEN);
		this->draw.set_xy(15, 20);
		cout << "游戏结束！";
		this->draw.set_xy(15, 22);
		cout << "游戏分数:" << this->number;
		end_game_music();
		Sleep(2500);
		play_game_music();
		return;
	}
	draw.set_color(COLOR_BLACK, FOREGROUND_RED);
	snake_draw(feel_postion.x, feel_postion.y,FEEL);

	int x_fist = ptemp->x;
	int y_fist = ptemp->y;
	this->draw.set_color(COLOR_BLACK, FOREGROUND_BLUE);
	snake_draw(ptemp->x, ptemp->y, SNAKE);
	int x_temp1;
	int y_temp1;

	while (ptemp->pNnext != NULL) {
		x_temp1 = ptemp->x;
		y_temp1 = ptemp->y;


	}
	snake_draw(x_temp,y_temp,NONE);
	game_map[y_temp][x_temp] = 0;

	if ((x_fist == feel_postion.x) && (y_fist == feel_postion.y)) {
		sanke_add();
		sanke_feel();
		this->number++;
		draw.set_xy(53, 9);
		this->draw.set_color(COLOR_BLACK, FOREGROUND_RED);
		cout << this->number;
		this->draw.set_color(COLOR_BLACK, FOREGROUND_BLUE);

	}

}

void Snake::sanke_pause(void)
{
	if (this->snake.snake_dir == 32) {
		this->game_status = game_status;
		snake.snake_dir == snake.snake_last_dir;

	}
}

void Snake::game_map_clear(void)
{
	for (int i = 1;i < GAME_HEIGHT - 1;i++) {
		for (int j = 0;j < GAME_WIDGET - 1;j++) {
			snake_draw(j, i, NONE);
			this->game_map[i][j] = 0;
		}
	}
}

void Snake::game_stop(void)
{
	switch (this->key_dir)
	{
	case 'a':
		this->stop_menu_choose--;
		if (this->stop_menu_choose < 0) {
			this->stop_menu_choose = 1;
		}
		this->stop_menu_choose %= 2;
		break;
	case 'd':
		this->stop_menu_choose++;
		this->stop_menu_choose %= 2;
		break;
	default:
		break;
	}
	if (this->key_dir == 13) {
		if (this->stop_menu_choose == 0) {
			this->number = 0;
			game_map_clear();
			sanke_game_init();
			this->game_status = GAME_START+1;
		}
		else {
			throw"game over";
		}
	}
	else{
		snake_stop_menu();
		this->menu = 1;
	}
}

void Snake::sanke_start_game(void)
{
	snake_clear();
	game_map_clear();
	sanke_game_init();
	this->game_status = GAME_START+1;
	while (1) {
		try {
			if (this->game_status == GAME_STOP+1) {
				if (this->three_mune_choose != 0) {
					throw "Game Over";
				}
				if (key_get(this->key_dir)) {
					game_stop();
				}
			}
			else {
				game_get_cmd();
				if (this->game_status == GAME_START+1) {
					sanke_start();
				}
				else if (this->game_status == GAME_PAUSE+1) {
					sanke_pause();
				}
			}
			
		}
		catch (...) {
			sanke_detele();
			cler_queue();
			break;
		}
	}
	this->menu = 1;
}

void Snake::snake_stop_menu(void)
{
	int one_color = 0;
	int two_color = 0;

	if (this->stop_menu_choose == 0) {
		one_color = FOREGROUND_RED;
		two_color = FOREGROUND_BLUE;
	}
	else if (this->stop_menu_choose == 1) {
		one_color = FOREGROUND_BLUE;
		two_color = FOREGROUND_RED;
	}
	int temp_row = WIDGET / 2 - DESC_OFFSET;
	int temp_col = HEIGHT / 3 + 5;
	this->draw.set_color(COLOR_BLACK, one_color);
	this->draw.set_xy(10, 24);
	cout << "重新玩一次";
	this->draw.set_color(COLOR_BLACK, two_color);
	this->draw.set_xy(20, 24);
	cout << "退出游戏";
}

void Snake::game_run(void)
{
	play_game_music();
	thread key_get(key_input);
	snake_init();

	while (1) {
		snake_main();
		sanke_start_game();
	}
}





