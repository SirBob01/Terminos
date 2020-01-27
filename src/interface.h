#ifndef TERMINOS_INTERFACE_H_
#define TERMINOS_INTERFACE_H_

#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <string>
#include <cstring>

#include "surface.h"

namespace Terminos {
    class Interface {
    public:
        static void set_title(std::string title);

        static void set_color(int color);
        
        static int get_color();

        static void toggle_cursor(bool enable);
        
        static void cursor_move(int x, int y);
        
        static bool write_at(std::string s, int x, int y);
        
        static void draw_surface(Surface *surface, int x, int y);

        static char get_keydown();
        
        static void pause();
        
        static void clear();
        
        static void close();
    };
}

#endif