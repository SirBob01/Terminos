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

    void Interface::draw_surface(Surface *surface, int x, int y) {
        bool dirty = surface->get_dirty();
        if(!dirty) {
            return;
        }

        int width = surface->get_width();
        int height = surface->get_height();
        int spacing = surface->get_spacing();
        
        std::string str;
        for(int i = 0; i < width; i++) {
            for(int j = 0; j < height; j++) {
                Termino cell = surface->get_at(i, j);
                str = cell.character;
                set_color(cell.color);
                write_at(str, x + i * (1 + spacing), y + j);
            }
        }
        surface->refresh();
        cursor_move(0, 0);
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