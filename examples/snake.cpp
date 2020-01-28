#include <iostream>
#include <deque>
#include <fstream>
#include <ctime>
#include <terminos/terminos.h>

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

void delay(int ms) { 
    clock_t start_time = clock(); 
   
    while (clock() < start_time + ms); 
}

// The snake is simply a dynamic queue
struct Vector2 {
    int x, y;
};

Vector2 random_position(int width, int height) {
    return {rand() % width, rand() % height};
}

void load_score(int *high_score) {
    std::ifstream save_file("snake.sav", std::ios::in);
    if(!save_file.good()) {
        std::ofstream write_save;
        write_save.open("snake.sav", std::ios::out);

        write_save << 0 << std::endl;
        *high_score = 0;
    }
    save_file >> *high_score;
    save_file.close();
}

void save_score(int score) {
    std::ofstream out_file;
    out_file.open("snake.sav", std::ios::out);
    out_file << score << std::endl;
}

int main(int argc, char **argv) {
    Terminos::Interface console;
    console.set_title("Snake!");

    srand(time(NULL));

    int size;
    console.write_at("Board size (Type a number)? ", 0, 0);
    std::cin >> size;
    std::cin.ignore();

    // Generate the board
    char arr[1] = {'.'};
    Terminos::Surface board(arr, size, size, Terminos::WHITE, true);

    // Generate the snake
    std::deque<Vector2> snake;
    snake.push_back({size/2, size/2});

    Vector2 head = snake.front();
    Vector2 apple = random_position(size, size);
    Vector2 c1, c2; // For lose condition when snake eats itself
    
    // Misc. variables
    char controls[64], score_card[256];
    char direction = RIGHT;
    
    int high_score;
    load_score(&high_score);

    int score = 0;
    bool lose = false;

    console.clear();

    while(!lose) {
        sprintf(controls, "%c%c%c%c to move.", UP, LEFT, DOWN, RIGHT);
        sprintf(score_card, "High score: %d | Score: %d", high_score, score);
        
        console.toggle_cursor(false);
        console.set_color(Terminos::WHITE);
        console.write_at(score_card, 4, 0);
        console.write_at(controls, 4, 1);

        // Controls
        char key_press = console.get_keydown();
        if(key_press == UP && direction != DOWN) {
            direction = UP;
        }
        else if(key_press == DOWN && direction != UP) {
            direction = DOWN;
        }
        else if(key_press == LEFT && direction != RIGHT) {
            direction = LEFT;
        }
        else if(key_press == RIGHT && direction != LEFT) {
            direction = RIGHT;
        }

        // Movement
        if(direction == UP) {
            head = {head.x, head.y-1};
        }
        else if(direction == DOWN) {
            head = {head.x, head.y+1};
        }
        else if(direction == LEFT) {
            head = {head.x-1, head.y};
        }
        else if(direction == RIGHT) {
            head = {head.x+1, head.y};
        }
        snake.push_back(head);

        // Score condition
        if(head.x == apple.x && head.y == apple.y) {
            score++;
            apple = random_position(size, size);
        }
        else {
            snake.pop_front();
        }

        // Lose conditions
        if(head.x < 0 || head.x >= size) {
            lose = 1;
        }
        else if(head.y < 0 || head.y >= size) {
            lose = 1;
        }
        if(snake.size() > 1) {
            for(int i = 0; i < snake.size(); i++) {
                for(int j = i + 1; j < snake.size(); j++) {
                    c1 = snake[i];
                    c2 = snake[j];
                    if(c1.x == c2.x && c1.y == c2.y) {
                        lose = true;
                    }
                }
            }
        } 

        // Render
        for(int i = 0; i < snake.size(); i++) {
            Vector2 cell = snake[i];
            board.set_at({'O', Terminos::YELLOW}, cell.x, cell.y);
        }
        if(direction == UP) {
            board.set_at({'^', Terminos::GREEN}, head.x, head.y);
        }
        else if(direction == DOWN) {
            board.set_at({'v', Terminos::GREEN}, head.x, head.y);
        }
        else if(direction == LEFT) {
            board.set_at({'<', Terminos::GREEN}, head.x, head.y);
        }
        else if(direction == RIGHT) {
            board.set_at({'>', Terminos::GREEN}, head.x, head.y);
        }
        board.set_at({'@', Terminos::RED}, apple.x, apple.y);
        console.draw_surface(&board, 4, 4);

        // Refresh console
        board.refresh();
        console.cursor_move(0, 0);

        delay(100);
    }
    save_score(score);
    console.set_color(Terminos::WHITE);
    console.write_at("GAME OVER! Press any key to continue...", 4, 2);
    
    console.pause();
    console.close();

    return 0;
}