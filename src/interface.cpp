#include "terminos.h"

#define INPUT_HANDLE GetStdHandle(STD_INPUT_HANDLE)
#define OUTPUT_HANDLE GetStdHandle(STD_OUTPUT_HANDLE)

using namespace std;

void strapp(char *string, char c) {
	int len = strlen(string);
	string[len] = c;
	string[len+1] = '\0';
}

void Interface::set_title(LPCSTR title) {
	SetConsoleTitle(title);
}

void Interface::cursor_move(int x, int y) {
	COORD pos = {(short)x, (short)y};
	SetConsoleCursorPosition(OUTPUT_HANDLE, pos);
}

void Interface::write_at(string s, int x, int y) {
	cursor_move(x, y);
	cout << s;
}

void Interface::draw_surface(Surface surface, int x, int y) {
	char **mask = surface.get_mask();

	int width = surface.get_width();
	int height = surface.get_height();
	int spacing = surface.get_spacing();
	
	/* Collapse board into single string to prevent
	rapid cursor movement in rendering (ew ugly) */
	int total = (x + ((spacing + 1) * width)) * height; 
	char *str = (char *)calloc(total*2, sizeof(char)); // More than enough memory!

	char *s = (char *)calloc(spacing, sizeof(char));
	char *l = (char *)calloc(x, sizeof(char));
	for(int i = 0; i < spacing; i++) {
		s[i] = ' ';
	}
	for(int i = 0; i < x; i++) {
		l[i] = ' ';
	}

	strcat(str, l);
	for(int j = 0; j < height; j++) {
		if(j > 0) {
			strcat(str, l);
		}
		for(int i = 0; i < width; i++) {
			strapp(str, mask[j][i]);
			strcat(str, s);
		}
		strapp(str, '\n');
	}
	write_at(str, 0, y);
}

int Interface::get_keydown() {
	if(kbhit()) {
		return (int)getch();
	}
}

void Interface::clear() {
	system("cls");
}

void Interface::close() {
	CloseHandle(INPUT_HANDLE);
	CloseHandle(OUTPUT_HANDLE);
}