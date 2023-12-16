#pragma once
#include "a8/util/Assert.h"
#include "a8/util/Callback3.h"
#include "a8/util/Debug.h"
#include "a8/util/Lang.h"
#include "a8/util/Math.h"
#include "a8/util/defines.h"
#include "debug.h"

namespace a8::util {

template <typename T>
class Buffer {
    using equals = bool (*)(T, T);

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

    Buffer(const T *buf, int len) { // copy constructor
        this->init();
        this->append(buf, len);
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
        if (this->hasIndex(idx)) {
            return this->buffer_[idx];
        }
        return def;
    }

    bool hasIndex(int idx) const {
        return idx >= 0 && idx < length_;
    }

    T get(int idx) const {
        if (idx < 0 || idx > this->length_ - 1) {
            Assert::illegalArgument(String() << "idx out of range.");
        }
        return this->buffer_[idx];
    }

    T getLast(T def) {
        if (this->length_ == 0) {
            return def;
        }
        return this->buffer_[this->length_ - 1];
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

    bool removeEle(T element) {
        return removeEle(
            element, [](T e1, T e2) { return e1 == e2; });
    }

    bool removeEle(T element, equals equals_) {
        int idx = this->indexOf(element, equals_);
        if (idx < 0) {
            return idx;
        }
        remove(idx);
        return idx;
    }

    bool remove(int idx) {
        if (idx < 0 || idx > this->length_) {
            return false;
        }
        T *buf2 = 0;
        int len2 = 0;
        int cap2 = 0;

        int leftLen = idx;
        int rightLen = this->length_ - idx - 1;

        Lang::append(buf2, len2, cap2, DELTA_BUF_CAP, 0, this->buffer_, 0, leftLen);
        Lang::append(buf2, len2, cap2, DELTA_BUF_CAP, 0, this->buffer_, idx + 1, rightLen);
        this->reset(buf2, len2, cap2);

        return true;
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

    int indexOf(T ele, bool (*equals)(T, T)) {
        for (int i = 0; i < length_; i++) {
            if (equals(ele, buffer_[i])) {
                return i;
            }
        }
        return -1;
    }

    int indexOf(T ele) {
        return indexOf(ele, [](T ele1, T ele2) { return ele1 == ele2; });
    }
    /**
     * return len or 0 if no data to read.
     */
    int read(int from1, T *buf, int len) {
        if (from1 >= this->length_) {
            return 0;
        }
        int len2 = Math::min(len, this->length_ - from1);
        Lang::copy<T>(this->buffer_, from1, len2, buf);
        return len2;
    }

    void reverse() {
        Lang::reverse<T>(this->buffer_, this->length_);
    }

    int write(T *buf, int bLen) {
        int len = Math::min(this->length_, bLen);
        Lang::copy(this->buffer_, 0, len, buf, 0);
        return len;
    }

    int toArray(T *buf, int len) {
        if (len != this->length_) {
            Assert::illegalArgument(String() << "buf length wrong.");
        }
        for (int i = 0; i < this->length_; i++) {
            buf[i] = this->buffer_[i];
        }
        return this->length_;
    }

    void clear() {
        this->length_ = 0;
    }

    void clear(void (*release)(T)) {
        this->forEach(release);
        this->clear();
    }

    Buffer<T> *append(Buffer<T> *buf, int from, int len) {
        return this->append(buf->buffer_, from, len);
    }

    Buffer<T> *append(const T &element) {
        T buf[1];
        buf[0] = element;
        return append(buf, 0, 1);
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
        A8_DEBUG(">>Buffer<T> *append(const T *elements, const int from, const int len)");
        Lang::append<T>(this->buffer_, this->length_, this->capacity_,
                        DELTA_BUF_CAP, 0,
                        elements, from, len);
        A8_DEBUG("<<Buffer<T> *append(const T *elements, const int from, const int len)");
        return this;
    }
    bool setIfNeeded(int idx, T emptyValue, T element) {
        if (this->isEquals(idx, element)) {
            return false;
        }
        set(idx, emptyValue, element);
        return true;
    }

    bool isEquals(int idx, const T &ele) {
        if (this->hasIndex(idx)) {
            return this->buffer_[idx] == ele;
        }
        return false;
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
            Assert::illegalArgument(String() << "idx out of bound.");
        }
        return this->buffer_[idx];
    }

    void operator=(T *array) {
    }

    Buffer<T> subBuffer(int from, int to) {
        Buffer<T> ret;
        if (from < to && to <= this->len_) {
            ret.append(this->buffer_, from, to - from);
        }
        return ret;
    }

    Buffer<T> subBuffer(int from) {
        Buffer<T> ret;
        int len2 = this->length_ - from;
        if (len2 > 0) {
            ret.append(this->buffer_, from, len2);
        }
        return ret;
    }

    template <typename C>
    void forEach(C c, void (*consumer)(C, T)) {
        for (int i = 0; i < this->length_; i++) {
            T ele = this->buffer_[i];
            consumer(c, ele);
        }
    }
    void forEach(void (*consumer)(T)) {
        for (int i = 0; i < this->length_; i++) {
            T ele = this->buffer_[i];
            consumer(ele);
        }
    }
};

// implementation

} // namespace a8::util
