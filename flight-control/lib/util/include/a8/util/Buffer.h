#pragma once
#include "a8/util/Assert.h"
#include "a8/util/Callback3.h"
#include "a8/util/ForEach.h"
#include "a8/util/Lang.h"
#include "a8/util/Math.h"
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
    Buffer() {
        this->init();
    }
    Buffer(const Buffer<T> &buf) { // copy constructor
        this->init();
        this->append(buf.buffer_, 0, buf.length_);
    }
    void operator=(const Buffer<T> &buf) { // assign operator
        this->clear();
        this->append(buf.buffer_, 0, buf.length_);
    }

    ~Buffer() {
        if (this->buffer_ != 0) {
            delete[] this->buffer_;
        }
    }
    template <typename S, typename C>
    Buffer<T> &append(const Buffer<S> &buf, const C context, T (*convert)(S, C)) {
        for (int i = 0; i < buf.len(); i++) {
            this.append(convert(buf[i], context));
        }
        return *this;
    }

    int length() const {
        return this->length_;
    }

    int len() const {
        return this->length_;
    }

    T get(int idx, T def) const {
        if (idx < 0 || idx >= length_) {
            return def;
        }
        return this->buffer_[idx];
    }

    T get(int idx) const {
        if (idx < 0 || idx > this->length_ - 1) {
            Assert::illegalArgument("idx out of range.");
        }
        return this->buffer_[idx];
    }
    T *buffer() const {
        return this->buffer_;
    }

    int removeIndexFrom(int from) {
        int removed = this->length_ - from;
        if (removed > 0) {
            this->length_ = from;
        } else {
            removed = 0;
        }
        return removed;
    }

    int removeEle(T element) {
        int idx = this->indexOf(element);
        if (idx < 0) {
            return -1;
        }

        T *buf2 = 0;
        int len2 = 0;
        int cap2 = 0;

        int leftLen = idx;
        int rightLen = this->length_ - idx - 1;

        Lang::append(buf2, len2, cap2, DELTA_BUF_CAP, 0, this->buffer_, 0, leftLen);
        Lang::append(buf2, len2, cap2, DELTA_BUF_CAP, 0, this->buffer_, idx + 1, rightLen);
        this->reset(buf2, len2, cap2);
        return 1;
    }

    void reset(T *buf, int len, int cap) {
        T *tmp = this->buffer_;
        this->buffer_ = buf;
        this->length_ = len;
        this->capacity_ = cap;
        delete tmp;
    }

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
        int len2 = Math::min(len, this->length_ - from1);
        Lang::copy<T>(this->buffer_, from1, len2, buf);
        return len2;
    }

    int toArray(T *buf, int len) {
        if (len != this->length_) {
            Assert::illegalArgument("buf length wrong.");
        }
        for (int i = 0; i < this->length_; i++) {
            buf[i] = this->buffer_[i];
        }
        return this->length_;
    }
    void clear() {
        this->length_ = 0;
    }

    Buffer<T> *append(const T &element) {
        return append(&element, 0, 1);
    }

    Buffer<T> *append(const T *elements, int length_) {
        return append(elements, 0, length_);
    }

    template <typename S>
    Buffer<T> *append(const S &element, T (*convert)(const S &)) {
        T ele = convert(element);
        return append(&ele, 0, 1);
    }

    Buffer<T> *append(const Buffer<T> &buf) {
        return append(buf.buffer_, 0, buf.length_);
    }

    Buffer<T> *append(const Buffer<T> *buf) {
        return append(buf->buffer_, 0, buf->length_);
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

    T operator[](int idx) {
        if (idx < 0 || idx > this->length_ - 1) {
            Assert::illegalArgument("idx out of bound.");
        }
        return this->buffer_[idx];
    }

    void operator=(T *array) {
    }
    Buffer<T> subBuffer(int from) {
        Buffer<T> ret;
        int len2 = this->length_ - from;
        if (len2 > 0) {
            ret.append(this->buffer_, from, len2);
        }
        return ret;
    }
};

// implementation

} // namespace a8::util
