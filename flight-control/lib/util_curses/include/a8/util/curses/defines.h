#pragma once

namespace a8::util::curses {

struct Rectangle {
    int x;
    int y;
    int h;
    int w;
    Rectangle(int x, int y, int h, int w) {
        this->x = x;
        this->y = y;
        this->h = h;
        this->w = w;
    }
};

} // namespace a8::util::curses