/* Don't forget to place terminos.h and all other source files
on the same directory as this file. When compiling, link this
file along with surface.cpp and interface.cpp */
#include <time.h>
#include "terminos.h"

#define MINE '@'
#define PLAYER '*'
#define HIDDEN '.'
#define REVEALED '_'
#define DIGITS "0123456789"
#define MINE_CHANCE 10

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define ACTIVATE 'e'

using namespace std;

struct Cell {
	short mine, reveal; // 1 or 0
	int x, y, adj;
};

struct Field {
	Cell **map;
	int width, height;
};

Field generate_map(int width, int height) {
	Cell **map = (Cell **)malloc(width * height * sizeof(Cell));
	for(int j = 0; j < height; j++) {
		map[j] = (Cell *)malloc(width * sizeof(Cell));
		for(int i = 0; i < width; i++) {
			if((int)(rand() % MINE_CHANCE) == 0) {
				map[j][i] = {1, 0, i, j, 0};
			}
			else {
				map[j][i] = {0, 0, i, j, 0};
			}
		}
	}
	return {map, width, height};
}

void calculate_adj(Field *field) {
	// Calculate the number displayed on cell adjacent to one or many mines.
	for(int y = 0; y < field->height; y++) {
		for(int x = 0; x < field->width; x++) {
			// Sides
			if(x-1 >= 0 && field->map[y][x-1].mine) {
				field->map[y][x].adj++;
			}
			if(x+1 < field->width && field->map[y][x+1].mine) {
				field->map[y][x].adj++;
			}
			if(y-1 >= 0 && field->map[y-1][x].mine) {
				field->map[y][x].adj++;
			}
			if(y+1 < field->height && field->map[y+1][x].mine) {
				field->map[y][x].adj++;
			}

			// Corners
			if(x-1 >= 0 && y-1 >= 0 && field->map[y-1][x-1].mine) {
				field->map[y][x].adj++;
			}
			if(x-1 >= 0 && y+1 < field->height && field->map[y+1][x-1].mine) {
				field->map[y][x].adj++;
			}
			if(x+1 < field->width && y-1 >= 0 && field->map[y-1][x+1].mine) {
				field->map[y][x].adj++;
			}
			if(x+1 < field->width && y+1 < field->height && field->map[y+1][x+1].mine) {
				field->map[y][x].adj++;
			}
		}
	}
}

void reveal(Field *field, int x, int y) {
	// Flood-fill algorithm to reveal all adjacent (non-"mine") cells, including corners.
	if(!(field->map[y][x].mine) && !(field->map[y][x].reveal)) {
		field->map[y][x].reveal = 1;
	}
	else {
		return;
	}

	if(x-1 < 0 || y-1 < 0 || x+1 > field->width-1 || y+1 > field->height-1) {
		return;
	}

	int left = field->map[y][x-1].mine;
	int right = field->map[y][x+1].mine;
	int top = field->map[y-1][x].mine;
	int bottom = field->map[y+1][x].mine;
	int topleft = field->map[y-1][x-1].mine;
	int bottomleft = field->map[y+1][x-1].mine;
	int topright = field->map[y-1][x+1].mine;
	int bottomright = field->map[y+1][x+1].mine;

	if(left || right || top || bottom || topleft || bottomleft || topright || bottomright) {
		return;
	}

	reveal(field, x-1, y);
	reveal(field, x, y-1);
	reveal(field, x+1, y);
	reveal(field, x, y+1);	
	reveal(field, x-1, y-1);
	reveal(field, x+1, y-1);
	reveal(field, x-1, y+1);
	reveal(field, x+1, y+1);
}

void reveal_all(Field *field) {
	for(int y = 0; y < field->height; y++) {
		for(int x = 0; x < field->width; x++) {
			field->map[y][x].reveal = 1;
		}
	}
}

void draw_minefield(Field field, Surface surface) {
	Cell c;
	for(int j = 0; j < field.height; j++) {
		for(int i = 0; i < field.width; i++) {
			c = field.map[j][i];
			if(c.reveal) {
				if(c.mine) {
					surface.set_at(MINE, i, j);
				}
				else if(c.adj > 0) {
					surface.set_at(DIGITS[c.adj], i, j);
				}
				else {
					surface.set_at(REVEALED, i, j);
				}
			}
		}
	}
}

int main() {
	srand(time(NULL));

	char key;
	int size;
	cout << "Board size (Type a number)? ";
	cin >> size;
	cin.ignore();

	char arr[1][1] = {{HIDDEN}};
	Surface board((char *)arr, size, size, 1);

	Field f = generate_map(size, size);
	calculate_adj(&f);

	int x = f.width/2;
	int y = f.height/2;
	int lose = 0;

	Interface::set_title("Minesweeper");
	Interface::clear();

	while(1) {
		// Controls
		Interface::cursor_move(4, 0);
		cout << UP  << LEFT << DOWN << RIGHT << " to move pointer.";
		Interface::cursor_move(4, 1);
		cout << ACTIVATE << " to activate cell.";

		key = (char)Interface::get_keydown();
		if(key == LEFT && x > 0) {
			x--;
		}
		if(key == RIGHT && x < f.width-1) {
			x++;
		}
		if(key == UP && y > 0) {
			y--;
		}
		if(key == DOWN && y < f.height-1) {
			y++;
		}
		if(key == ACTIVATE) {
			if(f.map[y][x].mine) {
				lose = 1;
			}
			else {
				reveal(&f, x, y);
			}
		}

		// Render
		draw_minefield(f, board);
		board.set_at(PLAYER, x, y);
		Interface::draw_surface(board, 4, 4);

		if(lose) {
			Interface::cursor_move(4, 2);
			cout << "Game over! Press any key to continue...";
			reveal_all(&f);
			draw_minefield(f, board);
			Interface::draw_surface(board, 4, 4);
			getch();
			break;
		}

		// Refresh console
		board.refresh();
		Interface::cursor_move(0, 0);
	}

	Interface::close();
	return 0;
}