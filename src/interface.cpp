#include "interface.h"

#define INPUT_HANDLE GetStdHandle(STD_INPUT_HANDLE)
#define OUTPUT_HANDLE GetStdHandle(STD_OUTPUT_HANDLE)

namespace Terminos {
    CONSOLE_SCREEN_BUFFER_INFO info;

    void Interface::set_title(std::string title) {
        SetConsoleTitle(
            static_cast<LPCSTR>(title.c_str())
        );
    }

    int Interface::get_color() {
        GetConsoleScreenBufferInfo(OUTPUT_HANDLE, &info);
        return info.wAttributes;
    }

    void Interface::set_color(int color) {
        if(get_color() == color) {
            return;
        }
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
        SetConsoleCursorPosition(
            OUTPUT_HANDLE, 
            {static_cast<short>(x), static_cast<short>(y)}
        );
    }

    bool Interface::write_at(std::string str, int x, int y) {
        int input_length = str.size();
        DWORD output_length;

        cursor_move(x, y);
        WriteConsole(
            OUTPUT_HANDLE, 
            str.c_str(), 
            input_length, 
            &output_length, 
            nullptr
        );

        return input_length == output_length;
    }

    void Interface::draw_termino(Termino cell, int x, int y) {
        std::string str;
        str = cell.character;
        set_color(cell.color);
        write_at(str, x, y);
    }

    void Interface::draw_surface(Surface *surface, int x, int y, 
                                 int spacing) {
        SHORT width = surface->width_ * (1 + spacing);
        SHORT height = surface->height_;

        CHAR_INFO map[width * height];
        for(int i = 0; i < width; i++) {
            for(int j = 0; j < height; j++) {
                Termino cell;
                if(spacing && i%(1 + spacing) == 0) {
                    cell = {' ', 0};
                }
                else {
                    cell = surface->get_at(i / (1 + spacing), j);
                }
                map[width * j + i].Char.AsciiChar = cell.character;
                map[width * j + i].Attributes = cell.color;
            }
        }
        SMALL_RECT dest = {
            static_cast<SHORT>(x), 
            static_cast<SHORT>(y), 
            static_cast<SHORT>(width+x),
            static_cast<SHORT>(height+y)
        };
        WriteConsoleOutput(
            OUTPUT_HANDLE, 
            map, 
            {width, height}, 
            {0, 0},
            &dest
        );
        surface->refresh();
    }

    char Interface::get_keydown() {
        if(kbhit()) {
            return static_cast<char>(getch());
        }
        return 0;
    }

    void Interface::pause() {
        getch();
    }

    void Interface::clear() {
        GetConsoleScreenBufferInfo(OUTPUT_HANDLE, &info);
        DWORD cell_count = info.dwSize.X * info.dwSize.Y;
        DWORD count;
        FillConsoleOutputCharacter(
            OUTPUT_HANDLE,
            ' ',
            cell_count,
            {0, 0},
            &count
        );
    }

    void Interface::close() {
        clear();
        CloseHandle(INPUT_HANDLE);
        CloseHandle(OUTPUT_HANDLE);
        FreeConsole();
    }
}