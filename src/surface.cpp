#include "surface.h"

namespace Terminos {
    Surface::Surface(char *arr, int width, int height, 
                     int color, bool flat, int spacing) {
        width_ = width;
        height_ = height;

        base_ = new Termino[width_ * height_];
        mask_ = new Termino[width_ * height_];

        // Fill the base "surface"
        for(int i = 0; i < width_; i++) {
            for(int j = 0; j < height_; j++) {
                if(flat) {
                    base_[width_ * j + i] = {
                        arr[0],
                        color
                    };
                }
                else {
                    base_[width_ * j + i] = {
                        arr[width_ * j + i], 
                        color
                    };
                }
            }
        }

        // Fill the mask
        for(int i = 0; i < width_; i++) {
            for(int j = 0; j < height_; j++) {
                set_at(base_[width_ * j + i], i, j);
            }
        }

        base_color_ = color;
        spacing_ = 1;
        dirty_ = true;
    }

    Surface::~Surface() {
        delete[] base_;
        delete[] mask_;
    }

    bool Surface::get_dirty() {
        return dirty_;
    }

    bool Surface::is_in_bounds(int x, int y) {
        if(x < 0 || x >= width_) {
            return false;
        }
        if(y < 0 || y >= height_) {
            return false;
        }
        return true;
    }

    int Surface::get_width() {
        return width_;
    }

    int Surface::get_height() {
        return height_;
    }

    Termino Surface::get_at(int x, int y) {
        return mask_[width_ * y + x];
    }

    Termino Surface::get_base_at(int x, int y) {
        return base_[width_ * y + x];
    }

    int Surface::get_base_color() {
        return base_color_;
    }

    int Surface::get_spacing() {
        return spacing_;
    }

    void Surface::set_char(char character, int x, int y) {
        Termino cell = get_at(x, y);
        if(is_in_bounds(x, y) && cell.character != character) {
            int index = width_ * y + x;
            mask_[index].character = character;
            dirty_ = true;
        }
    }

    void Surface::set_color(int color, int x, int y) {
        Termino cell = get_at(x, y);
        if(is_in_bounds(x, y) && cell.color != color) {
            int index = width_ * y + x;
            mask_[index].color = color;
            dirty_ = true;
        }
    }

    void Surface::set_at(Termino cell, int x, int y) {
        set_color(cell.color, x, y);
        set_char(cell.character, x, y);
    }

    void Surface::set_base_color(int color) {
        base_color_ = color;
    }

    void Surface::set_spacing(int spaces) {
        spacing_ = spaces;
    }

    void Surface::blit(Surface *source, int x, int y) {
        // Draw source surface on this surface at (x, y)
        for(int i = 0; i < source->get_width(); i++) {
            for(int j = 0; j < source->get_height(); j++) {
                set_at(
                    source->get_at(i, j),
                    x + i,
                    y + j
                );
            }
        }
    }

    void Surface::fill(Termino cell) {
        for(int i = 0; i < width_; i++) {
            for(int j = 0; j < height_; j++) {
                set_at(cell, i, j);
            }
        }
    }

    void Surface::refresh() {
        for(int i = 0; i < width_ * height_; i++) {
            mask_[i] = base_[i];
        }
        dirty_ = false;
    }
}
