#pragma once
#include "a8/util.h"

namespace a8::util {

template <typename T>
class Queue {

    Buffer<T> *buffer;
    int head;
    int tail;
    void neat() {
        if (head > this->len() * 10) {
            Buffer<T> *buf = this->buffer;
            this->buffer = new Buffer<T>();
            this->buffer->append(buf, head, tail - head);
            this->head = 0;
            this->tail = 0;
        }
    }

public:
    Queue() {
        this->buffer = new Buffer<T>();
        this->head = 0;
        this->tail = 0;
    }

    void append(T *buf, int len) {
        this->buffer->append(buf, len);
        this->tail += len;
    }

    void append(T ele) {
        this->buffer->append(ele);
        this->tail += 1;
    }

    int len() {
        return tail - head;
    }

    int take(T &ele) {
        if (len() == 0) {
            return -1;
        }
        ele = this->buffer->get(head);
        head++;
        return 1;
    }
};

} // namespace a8::util