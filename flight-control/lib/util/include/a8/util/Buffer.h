#pragma once
#include "a8/util/Callback3.h"
#include "a8/util/ForEach.h"
#include "a8/util/Lang.h"
#include "debug.h"

#define DELTA_BUF_CAP (16)

namespace a8::util {
template <typename T>
class Buffer {

private:
    int capacity;
    int length;

    T *buffer;

    void init() {
        this->length = 0;
        this->capacity = 0;
        this->buffer = 0;
    }

public:
    Buffer<T>() {
        this->init();
    }
    Buffer<T>(const Buffer<T> &buf) { // copy constructor
        this->init();
        this->append(buf.buffer, 0, buf.length);
    }
    void operator=(const Buffer<T> &buf) { // assign operator
        this->clear();
        this->append(buf.buffer, 0, buf.length);
    }

    ~Buffer<T>() {
        if (this->buffer != 0) {
            // delete[] this->buffer;
        }
    }

    int getLength() const;

    int len() const {
        return this->length;
    }

    T get(int idx) const;
    T *getAll() const;
    int remove(int from);

    bool isEmpty() {
        return this->length == 0;
    }

    int indexOf(T ele) {
        for (int i = 0; i < length; i++) {
            if (ele == buffer[i]) {
                return i;
            }
        }
        return -1;
    }

    int read(int from1, T *buf, int len) {
        if (from1 >= this->length) {
            return -1;
        }
        if (len > this->length - from1) {
            len = this->length - from1;
        }
        Lang::copy<T>(this->buffer, from1, len, buf);
        return len;
    }

    T *toArray(T *buf) {
        for (int i = 0; i < this->length; i++) {
            buf[i] = this->buffer[i];
        }
        return buf;
    }
    void clear() {
        this->length = 0;
    }

    Buffer<T> *append(const T &element) {
        return append(&element, 1);
    }

    Buffer<T> *append(const T *elements, int length) {
        return append(elements, 0, length);
    }

    Buffer<T> *append(const Buffer<T> &buf) {
        return append(buf.buffer, 0, buf.length);
    }

    Buffer<T> *append(const T *elements, const int from, const int len) {
        LOG(">>Buffer<T> *append(const T *elements, const int from, const int len)");
        Lang::append<T>(this->buffer, this->length, this->capacity,
                        DELTA_BUF_CAP, 0,
                        elements, from, len);
        LOG2("this->buffer:", this->buffer);
        LOG("<<Buffer<T> *append(const T *elements, const int from, const int len)");
        return this;
    }
    void set(int idx, T emptyValue, T element) {
        while (this->length - 1 < idx) {
            this->append(emptyValue);
        }
        this->buffer[idx] = element;
    }
    void set(int idx, T element) {
        this->buffer[idx] = element;
    }
    int removeLast() {
        if (this->length == 0) {
            return false;
        }
        this->length--;
        return 1;
    }
};

// implementation

template <typename T>
T Buffer<T>::get(int idx) const {
    return this->buffer[idx];
}

template <typename T>
int Buffer<T>::getLength() const {
    return this->length;
}

template <typename T>
T *Buffer<T>::getAll() const {
    return this->buffer;
}

template <typename T>
int Buffer<T>::remove(int from) {
    int removed = this->length - from;
    if (removed > 0) {
        this->length = from;
    } else {
        removed = 0;
    }
    return removed;
}

} // namespace a8::util
