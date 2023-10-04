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
    int capacity_;
    int length_;

    T *buffer_;

    void init() {
        this->length_ = 0;
        this->capacity_ = 0;
        this->buffer_ = 0;
    }

public:
    Buffer<T>() {
        this->init();
    }
    Buffer<T>(const Buffer<T> &buf) { // copy constructor
        this->init();
        this->append(buf.buffer_, 0, buf.length_);
    }
    void operator=(const Buffer<T> &buf) { // assign operator
        this->clear();
        this->append(buf.buffer_, 0, buf.length_);
    }

    ~Buffer<T>() {
        if (this->buffer_ != 0) {
            // delete[] this->buffer_;
        }
    }

    int length() const;

    int len() const {
        return this->length_;
    }

    T get(int idx) const;
    T *getAll() const;
    int remove(int from);

    bool isEmpty() {
        return this->length_ == 0;
    }

    int indexOf(T ele) {
        for (int i = 0; i < length_; i++) {
            if (ele == buffer_[i]) {
                return i;
            }
        }
        return -1;
    }

    int read(int from1, T *buf, int len) {
        if (from1 >= this->length_) {
            return -1;
        }
        if (len > this->length_ - from1) {
            len = this->length_ - from1;
        }
        Lang::copy<T>(this->buffer_, from1, len, buf);
        return len;
    }

    T *toArray(T *buf) {
        for (int i = 0; i < this->length_; i++) {
            buf[i] = this->buffer_[i];
        }
        return buf;
    }
    void clear() {
        this->length_ = 0;
    }

    Buffer<T> *append(const T &element) {
        return append(&element, 1);
    }

    Buffer<T> *append(const T *elements, int length_) {
        return append(elements, 0, length_);
    }

    Buffer<T> *append(const Buffer<T> &buf) {
        return append(buf.buffer_, 0, buf.length_);
    }

    Buffer<T> *append(const T *elements, const int from, const int len) {
        LOG(">>Buffer<T> *append(const T *elements, const int from, const int len)");
        Lang::append<T>(this->buffer_, this->length_, this->capacity_,
                        DELTA_BUF_CAP, 0,
                        elements, from, len);
        LOG2("this->buffer_:", this->buffer_);
        LOG("<<Buffer<T> *append(const T *elements, const int from, const int len)");
        return this;
    }
    void set(int idx, T emptyValue, T element) {
        while (this->length_ - 1 < idx) {
            this->append(emptyValue);
        }
        this->buffer_[idx] = element;
    }
    void set(int idx, T element) {
        this->buffer_[idx] = element;
    }
    int removeLast() {
        if (this->length_ == 0) {
            return false;
        }
        this->length_--;
        return 1;
    }
};

// implementation

template <typename T>
T Buffer<T>::get(int idx) const {
    return this->buffer_[idx];
}

template <typename T>
int Buffer<T>::length() const {
    return this->length_;
}

template <typename T>
T *Buffer<T>::getAll() const {
    return this->buffer_;
}

template <typename T>
int Buffer<T>::remove(int from) {
    int removed = this->length_ - from;
    if (removed > 0) {
        this->length_ = from;
    } else {
        removed = 0;
    }
    return removed;
}

} // namespace a8::util
