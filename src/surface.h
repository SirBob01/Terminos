#ifndef TERMINOS_SURFACE_H_
#define TERMINOS_SURFACE_H_

#include <cstdlib>
#include <string>
#include <vector>

namespace Terminos {
    // Pre-defined colors
    enum Color {
        RED = 4,
        GREEN = 2,
        BLUE = 1,
        AQUA = 3,
        YELLOW = 6,
        PURPLE = 5,
        BLACK = 0,
        WHITE = 7,
        GRAY = 8,
        LIGHT_BLUE = 9,
        LIGHT_GREEN = 0xA,
        LIGHT_AQUA = 0xB,
        LIGHT_RED = 0xC,
        LIGHT_YELLOW = 0xE,
        BRIGHT_WHITE = 0xF
    };

    // Simple character-color pairing
    struct Termino {
        char character;
        int color;
    };

    // Represent a 2D surface
    class Surface {
        Termino *base_;
        Termino *mask_;

        int width_;
        int height_;
        
        int base_color_;
        int spacing_;

        bool dirty_;

        bool get_dirty();

        friend class Interface;

    public:

        Surface(char *arr, int width, int height, 
                int color, bool flat=false, int spacing=1);
        ~Surface();

        bool is_in_bounds(int x, int y);

        int get_width();
        
        int get_height();
        
        Termino get_at(int x, int y);

        Termino get_base_at(int x, int y);

        int get_base_color();

        int get_spacing();

        void set_char(char character, int x, int y);
        
        void set_color(int color, int x, int y);

        void set_at(Termino cell, int x, int y);
        
        void set_base_color(int color);
        
        void set_spacing(int spaces);
        
        void blit(Surface *source, int x, int y);
        
        void fill(Termino cell);

        void refresh();
    };
}

#endif