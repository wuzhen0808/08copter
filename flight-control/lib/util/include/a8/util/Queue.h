#pragma once
#include "a8/util.h"

namespace a8::util {

template <typename T>
class Queue {

    Buffer<T> *buffer;
    int head;
    void freeSpaceIfNecessary() {

        int len = this->len();
        if (head > 512 && len < head) {
            Buffer<T> *buf0 = this->buffer;
            this->buffer = new Buffer<T>();
            this->buffer->append(buf0, head, len);
            delete buf0;
            this->head = 0;
        }
    }

public:
    Queue() {
        this->buffer = new Buffer<T>();
        this->head = 0;
    }
    ~Queue() {
        delete this->buffer;
    }

    void append(const Buffer<T> &buf) {
        this->append(buf.buffer(), buf.len());
    }

    bool isEmpty() {
        return this->len() == 0;
    }

    void append(T *buf, int len) {
        this->buffer->append(buf, len);
    }

    void append(T ele) {
        this->buffer->append(ele);        
    }

    int len() {
        return this->buffer->len() - head;
    }

    /**
     * return the len or 0
     */
    int take(T *buf, int bufLen) {
        int len = this->buffer->read(head, buf, bufLen);
        if (len > 0) {
            this->head += len;
            this->freeSpaceIfNecessary();
        }
        return len;
    }
    int take(T &ele) {
        return take(&ele, 1);
    }
};

} // namespace a8::util