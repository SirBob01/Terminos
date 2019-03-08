#include "terminos.h"

using namespace std;

Surface::Surface(char *arr, int width, int height, int is_flat) {
	base = (char **)malloc(width * height * sizeof(char));
	mask = (char **)malloc(width * height * sizeof(char));

	char **board = (char **)malloc(width * height * sizeof(char));
	for(int j = 0; j < height; j++) {
		char *row = (char *)malloc(width * sizeof(char));
		for(int i = 0; i < width; i++) {
			if(is_flat) {
				row[i] = arr[0];
			}
			else {
				row[i] = arr[width*j + i];
			}
		}
		board[j] = row;
	}

	for(int y = 0; y < height; y++) {
		base[y] = (char *)malloc(width * sizeof(char));
		mask[y] = (char *)malloc(width * sizeof(char));

		memcpy(base[y], board[y], width);
		memcpy(mask[y], board[y], width);
	}

	dimensions[0] = width;
	dimensions[1] = height;
	
	spacing = 1;
}

char **Surface::get_base() {
	return base;
}

char **Surface::get_mask() {
	return mask;
}

int Surface::get_width() {
	return dimensions[0];
}

int Surface::get_height() {
	return dimensions[1];
}

int Surface::get_spacing() {
	return spacing;
}

char Surface::get_at(int x, int y) {
	return mask[y][x];
}

void Surface::set_at(char character, int x, int y) {
	if(x < 0 || x >= dimensions[0]) {
		return;
	}
	if(y < 0 || y >= dimensions[1]) {
		return;
	}
	mask[y][x] = character;
}

void Surface::set_spacing(int spaces) {
	spacing = spaces;
}

void Surface::blit(Surface source, int x, int y) {
	// Draw source surface on this surface at (x, y)
	char **surf = source.get_mask();
	for(int j = 0; j < source.get_height(); j++) {
		for(int i = 0; i < source.get_width(); i++) {
			if(x+i < 0 || x+i >= dimensions[0]) {
				continue;
			}
			if(y+j < 0 || y+j >= dimensions[1]) {
				continue;
			}
			set_at(surf[j][i], x+i, y+j);
		}
	}
}

void Surface::refresh() {
	free(mask);
	mask = (char **)calloc(dimensions[0]*dimensions[1], sizeof(char));
	for(int y = 0; y < dimensions[1]; y++) {
		free(mask[y]);
		mask[y] = (char *)calloc(dimensions[0], sizeof(char));
		memcpy(mask[y], base[y], dimensions[0]);
	}
}