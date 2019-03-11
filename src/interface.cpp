#include "terminos.h"

#define INPUT_HANDLE GetStdHandle(STD_INPUT_HANDLE)
#define OUTPUT_HANDLE GetStdHandle(STD_OUTPUT_HANDLE)

using namespace std;

CONSOLE_SCREEN_BUFFER_INFO info;

void Interface::set_title(LPCSTR title) {
	SetConsoleTitle(title);
}

int Interface::get_color() {
	GetConsoleScreenBufferInfo(OUTPUT_HANDLE, &info);
	return info.wAttributes;
}

void Interface::set_color(int color) {
	SetConsoleTextAttribute(OUTPUT_HANDLE, color);
}

void Interface::toggle_cursor(bool enable) {
	CONSOLE_CURSOR_INFO c;
	GetConsoleCursorInfo(OUTPUT_HANDLE, &c);

	c.dwSize = 1;
	c.bVisible = false;
	SetConsoleCursorInfo(OUTPUT_HANDLE, &c);
}

void Interface::cursor_move(int x, int y) {
	SetConsoleCursorPosition(OUTPUT_HANDLE, {(short)x, (short)y});
}

bool Interface::write_at(string s, int x, int y) {
	int real = s.size();

	// Wastes space with excess DOUBLED memory, but it's safe
	// Gotta find a better arbitrary safety buffer
	char *character_buffer = (char *)malloc(real*sizeof(char)*2);
	strcpy(character_buffer, s.c_str());

	DWORD output_buffer;

	cursor_move(x, y);
	WriteConsole(OUTPUT_HANDLE, character_buffer, real, &output_buffer, NULL);
	free(character_buffer);

	return real == output_buffer;
}

void Interface::draw_surface(Surface surface, int x, int y) {
	if(!surface.is_dirty()) {
		return;
	}
	int width = surface.get_width();
	int height = surface.get_height();
	int spacing = surface.get_spacing();
	
	char c[2];
	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height; j++) {
			c[0] = surface.get_char(i, j);
			c[1] = '\0';
			set_color(surface.get_color(i, j));
			write_at(c, x+i*(1+spacing), y+j);
		}
	}
	surface.set_dirty(false);
}

int Interface::get_keydown() {
	if(kbhit()) {
		return (int)getch();
	}
}

void Interface::pause() {
	getch();
}

void Interface::clear() {
	system("cls");
}

void Interface::close() {
	clear();
	CloseHandle(INPUT_HANDLE);
	CloseHandle(OUTPUT_HANDLE);
	FreeConsole();
}