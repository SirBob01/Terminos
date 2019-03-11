#include "terminos.h"

using namespace std;

Surface::Surface(char *arr, int width, int height, int color, bool is_flat) {
	base = (Termino *)malloc(width*height*sizeof(Termino));
	mask = (Termino *)malloc(width*height*sizeof(Termino));

	for(int i = 0; i < width; i++) {
		for(int j = 0; j < height; j++) {
			if(is_flat) {
				base[width*j + i] = {arr[0], color};
			}
			else {
				base[width*j + i] = {arr[width*j + i], color};
			}
		}
	}

	// Copy the contents of the base to mask
	memcpy(mask, base, width*height*sizeof(Termino));

	dimensions[0] = width;
	dimensions[1] = height;
	
	default_color = color;
	spacing = 1;

	dirty = true;
}

Termino *Surface::get_base() {
	return base;
}

Termino *Surface::get_mask() {
	return mask;
}

int Surface::get_width() {
	return dimensions[0];
}

int Surface::get_height() {
	return dimensions[1];
}

int Surface::get_default_color() {
	return default_color;
}

int Surface::get_spacing() {
	return spacing;
}

char Surface::get_char(int x, int y) {
	return mask[dimensions[0]*y + x].character;
}

int Surface::get_color(int x, int y) {
	return mask[dimensions[0]*y + x].color;
}

bool Surface::is_dirty() {
	return dirty;
}

void Surface::set_dirty(bool d) {
	dirty = d;
}

bool Surface::within_bounds(int x, int y) {
	if(x < 0 || x >= dimensions[0]) {
		return false;
	}
	if(y < 0 || y >= dimensions[1]) {
		return false;
	}
	return true;
}

void Surface::set_char(char c, int x, int y) {
	if(within_bounds(x, y) && get_char(x, y) != c) {
		mask[dimensions[0]*y + x].character = c;
		set_dirty(true);
	}
}

void Surface::set_color(int c, int x, int y) {
	if(within_bounds(x, y) && get_color(x, y) != c) {
		mask[dimensions[0]*y + x].color = c;
		set_dirty(true);
	}
}

void Surface::set_at(char character, int color, int x, int y) {
	set_color(color, x, y);
	set_char(character, x, y);
}

void Surface::set_spacing(int spaces) {
	spacing = spaces;
}

void Surface::set_default_color(int color) {
	default_color = color;
}

void Surface::blit(Surface source, int x, int y) {
	// Draw source surface on this surface at (x, y)
	for(int i = 0; i < source.get_width(); i++) {
		for(int j = 0; j < source.get_height(); j++) {
			set_at(source.get_char(i, j), source.get_color(i, j), x+i, y+j);
		}
	}
}

void Surface::reset_colors() {
	for(int i = 0; i < dimensions[0]; i++) {
		for(int j = 0; j < dimensions[1]; j++) {
			set_color(default_color, i, j);
		}
	}
}

void Surface::refresh() {
	// Free memory
	free(mask);
	mask = (Termino *)malloc(dimensions[0]*dimensions[1]*sizeof(Termino));
	memcpy(mask, base, dimensions[0]*dimensions[1]*sizeof(Termino));
	set_dirty(true);
}