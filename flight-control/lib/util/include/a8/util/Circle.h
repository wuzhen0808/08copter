#pragma once
#include "a8/util/Buffer.h"

namespace a8::util {

template <typename T>
class Circle {
    int cap;
    int len;
    T *buf_;

public:
    Circle(int cap, T def) {
        this->cap = cap;
        this->buf_ = new T[cap];
        this->len = 0;
        for (int i = 0; i < cap; i++) {
            buf_[i] = def;
        }
    }

    ~Circle() {
        delete[] this->buf_;
    }

    void add(const T ele) {
        this->buf_[len % cap] = ele;
        len++;
    }
    
    T get(int idx) {
        return this->buf_[idx % cap];
    }
};

// implementation

} // namespace a8::util
