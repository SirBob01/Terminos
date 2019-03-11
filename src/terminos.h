#ifndef TERMINOS_H
#define TERMINOS_H

#include <string>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

// Define the colors
#define RED 4
#define GREEN 2
#define BLUE 1
#define AQUA 3
#define YELLOW 6
#define PURPLE 5
#define BLACK 0
#define WHITE 7
#define GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 0xA
#define LIGHT_AQUA 0xB
#define LIGHT_RED 0xC
#define LIGHT_YELLOW 0xE
#define BRIGHT_WHITE 0xF

struct Termino {
	// Simple character-color pairing
	char character;
	int color;
};

class Surface {
	Termino *base;
	Termino *mask;

	int dimensions[2];
	int default_color;
	int spacing;

	bool dirty;

public:
	Surface(char *arr, int width, int height, int color, bool is_flat);
	Termino *get_base();
	Termino *get_mask();

	int get_width();
	int get_height();
	
	char get_char(int x, int y);
	int get_color(int x, int y);

	int get_default_color();
	int get_spacing();

	bool is_dirty();
	void set_dirty(bool d);

	bool within_bounds(int x, int y);

	void set_char(char character, int x, int y);
	void set_color(int color, int x, int y);
	void set_at(char character, int color, int x, int y);
	
	void set_default_color(int color);
	void set_spacing(int spaces);
	
	void blit(Surface source, int x, int y);
	void reset_colors();
	void refresh();
};

class Interface {
public:
	static void set_title(LPCSTR title);
	static void set_color(int color);
	static int get_color();

	static void toggle_cursor(bool enable);
	static void cursor_move(int x, int y);
	static bool write_at(std::string s, int x, int y);
	static void draw_surface(Surface surface, int x, int y);

	static int get_keydown();
	static void pause();
	static void clear();
	static void close();
};

#endif