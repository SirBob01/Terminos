#include <iostream>
#include <ctime>
#include <terminos/terminos.h>

#define MINE '@'
#define PLAYER '*'
#define HIDDEN '.'
#define REVEALED '_'
#define DIGITS "0123456789"
#define MINE_PROBABILITY 10

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define ACTIVATE 'e'

struct Cell {
    short mine, reveal; // 1 or 0
    int x, y, adj;
};

struct Field {
    Cell **map;
    int width, height;
};

Field create_map(int width, int height) {
    Cell **map = new Cell *[width * height];
    for(int j = 0; j < height; j++) {
        map[j] = new Cell[width];
        for(int i = 0; i < width; i++) {
            if((int)(rand() % MINE_PROBABILITY) == 0) {
                map[j][i] = {1, 0, i, j, 0};
            }
            else {
                map[j][i] = {0, 0, i, j, 0};
            }
        }
    }
    return {map, width, height};
}

void destroy_map(Field field) {
    for(int i = 0; i < field.height; i++) {
        delete[] field.map[i];
    }
    delete[] field.map;
}

void calculate_adj(Field *field) {
    // Calculate the number displayed on cell adjacent to one or many mines.
    for(int x = 0; x < field->width; x++) {
        for(int y = 0; y < field->height; y++) {
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
    for(int x = 0; x < field->width; x++) {
        for(int y = 0; y < field->height; y++) {
            field->map[y][x].reveal = 1;
        }
    }
}

void draw_minefield(Field field, Terminos::Surface *surface) {
    Cell c;
    for(int i = 0; i < field.width; i++) {
        for(int j = 0; j < field.height; j++) {
            c = field.map[j][i];
            if(c.reveal) {
                if(c.mine) {
                    surface->set_at({MINE, Terminos::PURPLE}, i, j);
                }
                else if(c.adj == 1) {
                    surface->set_at({DIGITS[c.adj], Terminos::LIGHT_BLUE}, i, j);
                }
                else if(c.adj == 2) {
                    surface->set_at({DIGITS[c.adj], Terminos::LIGHT_GREEN}, i, j);
                }
                else if(c.adj >= 3) {
                    surface->set_at({DIGITS[c.adj], Terminos::LIGHT_RED}, i, j);
                }
                else {
                    surface->set_at({REVEALED, Terminos::LIGHT_YELLOW}, i, j);
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    Terminos::Interface console;
    console.set_title("Minesweeper!");
    
    srand(time(nullptr));

    int size;
    console.write_at("Board size (Type a number)? ", 0, 0);
    std::cin >> size;
    std::cin.ignore();

    // Generate the board
    char arr[1] = {HIDDEN};
    Terminos::Surface board(arr, size, size, Terminos::WHITE, true);

    // Generate the map
    Field f = create_map(board.get_width(), board.get_height());
    calculate_adj(&f);

    // Misc. variables
    char key, controlsA[32], controlsB[32];
    int x = f.width/2;
    int y = f.height/2;
    bool lose = false;

    console.clear();

    while(!lose) {
        // Instructions
        sprintf(controlsA, "%c%c%c%c to move pointer.", UP, LEFT, DOWN, RIGHT);
        sprintf(controlsB, "%c to activate cell.", ACTIVATE);

        console.toggle_cursor(false);
        console.set_color(Terminos::WHITE);
        console.write_at(controlsA, 4, 0);
        console.write_at(controlsB, 4, 1);

        // Controls
        key = console.get_keydown();
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
            // Lose condition
            if(f.map[y][x].mine) {
                lose = true;
            }
            else {
                reveal(&f, x, y);
            }
        }

        // Render the cells
        draw_minefield(f, &board);
        board.set_at({PLAYER, Terminos::BRIGHT_WHITE}, x, y);
        
        // Draw the whole board
        console.draw_surface(&board, 4, 4);
    }

    console.clear();
    console.set_color(Terminos::WHITE);
    console.write_at("GAME OVER! Press any key to continue...", 4, 2);
    
    reveal_all(&f);
    draw_minefield(f, &board);
    console.draw_surface(&board, 4, 4);

    destroy_map(f);

    console.pause();
    console.close();

    return 0;
}