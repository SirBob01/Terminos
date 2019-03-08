#include <time.h>
#include "Terminos/terminos.h"

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

using namespace std;

/* SNAKE! */
// Snake is a dynamic stack (first in, first out)
struct Vector2 {
	int x, y;
};

struct Snake {
	int length = 0;
	int MAX = 1;
	Vector2 *arr = (Vector2 *)malloc(MAX * sizeof(Vector2));
};

Vector2 get(Snake *s, int index) {
	if(index >= s->length) {
		cout << "Index of range!" << "\n";
		return {-1, -1};
	}
	return s->arr[index];
}

Vector2 pop(Snake *s) {
	// FILO
	Vector2 first = s->arr[0];
	for(int i = 0; i < s->length-1; i++) {
		s->arr[i] = s->arr[i+1];
	}
	s->length--;
	return first;
}

Vector2 push(Snake *s, Vector2 cell) {
	if(s->length >= s->MAX) {
		s->MAX += 5;
		realloc(s->arr, s->MAX);
	}
	s->length++;
	s->arr[s->length-1] = cell;
	return s->arr[s->length-1];
}

int randrange(int x) {
	// Generates a random number between 0 and x - 1
	return rand() % x;
}

Vector2 random_position(int width, int height) {
	return {randrange(width), randrange(height)};
}

void delay(float seconds) {
	int milliseconds = 1000*seconds;
	clock_t start = clock();
	while(clock() < start+milliseconds);
}

int main() {
	srand(time(NULL));

	int size;
	cout << "Board size (Type a number)? ";
	cin >> size;

	char direction = RIGHT;
	char key;

	char score_card[1024];
	int score = 0;
	int high_score = 0;
	int lose = 0;

	Interface display;
	display.set_title("Snake!");

	char board[1][1] = {{'.'}};
	Surface surface((char *)board, size, size, 1);

	Snake snake;
	push(&snake, {size/2, size/2});
	
	Vector2 head = get(&snake, snake.length-1);
	Vector2 apple = random_position(size, size);
	Vector2 c1, c2; // For lose condition when snake eats itself

	display.clear();

	while(lose == 0) {
		sprintf(score_card, "High score: %d | Score: %d", score, high_score);
		display.write_at(score_card, 0, 0);

		// Controls
		key = (char)display.get_keydown();
		if(key == UP && direction != DOWN) {
			direction = UP;
		}
		else if(key == DOWN && direction != UP) {
			direction = DOWN;
		}
		else if(key == LEFT && direction != RIGHT) {
			direction = LEFT;
		}
		else if(key == RIGHT && direction != LEFT) {
			direction = RIGHT;
		}

		// Movement
		if(direction == UP) {
			head = push(&snake, {head.x, head.y-1});
		}
		else if(direction == DOWN) {
			head = push(&snake, {head.x, head.y+1});
		}
		else if(direction == LEFT) {
			head = push(&snake, {head.x-1, head.y});
		}
		else if(direction == RIGHT) {
			head = push(&snake, {head.x+1, head.y});
		}

		// Score condition
		if(head.x == apple.x && head.y == apple.y) {
			score++;
			apple = random_position(size, size);
		}
		else {
			pop(&snake);
		}

		// Lose conditions
		if(head.x < 0 || head.x >= size) {
			lose = 1;
		}
		else if(head.y < 0 || head.y >= size) {
			lose = 1;
		}
		if(snake.length > 1) {
			for(int i = 0; i < snake.length; i++) {
				for(int j = i + 1; j < snake.length; j++) {
					c1 = get(&snake, i);
					c2 = get(&snake, j);
					if(c1.x == c2.x && c1.y == c2.y) {
						lose = 1;
					}
				}
			}
		} 

		// Render
		for(int i = 0; i < snake.length; i++) {
			Vector2 cell = get(&snake, i);
			surface.set_at('O', cell.x, cell.y);
		}
		if(direction == UP) {
			surface.set_at('^', head.x, head.y);
		}
		else if(direction == DOWN) {
			surface.set_at('v', head.x, head.y);
		}
		else if(direction == LEFT) {
			surface.set_at('<', head.x, head.y);
		}
		else if(direction == RIGHT) {
			surface.set_at('>', head.x, head.y);
		}
		surface.set_at('@', apple.x, apple.y);

		display.draw_surface(surface, 3, 2);
		surface.refresh();
		display.cursor_move(0, 0);

		delay(0.1);
	}

	display.clear();
	cout << "Game over!" << "\n";
	cout << "Press any key to exit...";
	getch();
	display.close();
	return 0;
}