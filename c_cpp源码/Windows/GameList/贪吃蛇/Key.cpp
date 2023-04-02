#include "Key.h"

static queue<int> key_data;

void key_input()
{
	char rec_c;
	while (rec_c = _getch()) {
		if ((rec_c == 'a') || (rec_c == 'w') || (rec_c == 's') || (rec_c == 'd') || (rec_c == 13) || (rec_c == 27) || (rec_c == 32)) {
			key_data.push(rec_c);
		}
	}
}

int key_get(char& data)
{
	Sleep(10);
	if (!key_data.empty()) {
		data = key_data.front();
		key_data.pop();
		return 1;
	}
	else {
		return 0;
	}
}

void cler_queue(void)
{
	int cout = key_data.size();
	for (int i = 0; i < cout;i++) {
		key_data.pop();
	}
}

void dir_test(char& c1, char& c2)
{
	int sum = 0;

	sum = c1 + c2;
	if ((sum != 234) && (sum != 197)) {
		c2 = c1;
	}
}
