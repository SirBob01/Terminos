#ifndef TERMINOS_H
#define TERMINOS_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

void strapp(char *string, char c);

class Surface {
	char **base;
	char **mask;
	int dimensions[2];
	int spacing;

public:
	Surface(char *arr, int width, int height, int is_flat);
	char **get_base();
	char **get_mask();
	int get_width();
	int get_height();
	int get_spacing();
	char get_at(int x, int y);
	void set_at(char character, int x, int y);
	void set_spacing(int spaces);
	void blit(Surface source, int x, int y);
	void refresh();
};

class Interface {
public:
	static void set_title(LPCSTR title);
	static void cursor_move(int x, int y);
	static void write_at(std::string s, int x, int y);
	static void draw_surface(Surface surface, int x, int y);
	static int get_keydown();
	static void clear();
	static void close();
};

#endif