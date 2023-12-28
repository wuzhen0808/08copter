#pragma once
#include "a8/util/Array.h"
#include "a8/util/Assert.h"
#include "a8/util/Callback3.h"
#include "a8/util/Debug.h"
#include "a8/util/Iterator.h"
#include "a8/util/Lang.h"
#include "a8/util/Math.h"
#include "a8/util/defines.h"
#include "debug.h"

#ifndef A8_BUFFER_DEBUG
#define A8_BUFFER_DEBUG (0)
#endif
namespace a8::util {

template <typename T>
class Buffer {
public:
    template <typename X>
    class Iterator : public a8::util::Iterator<X> {
    public:
        Buffer<X> *buffer;
        int idx;
        Iterator(Buffer<X> *buffer) {
            this->buffer = buffer;
            this->idx = 0;
        }
        bool hasNext() override {
            return this->idx < this->buffer->len();
        }

        X next() {
            return this->buffer->buffer_[idx++];
        }
    };
    using equals = bool (*)(T, T);

public:
    template <typename X>
    static X *get(Buffer<X *> *buffer, int idx, X *def) {
        return buffer->get(idx, def);
    }

    template <typename X>
    static X *get(Buffer<X *> *buffer, int idx) {
        return buffer->get(idx, 0);
    }

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
    template <typename C>
    void sort(C c, int (*compare)(C, T, T)) {
        for (int i = 0; i < this->length_; i++) {
            for (int j = i + 1; j < this->length_; j++) {
                if (compare(c, this->buffer_[j], this->buffer_[i]) < 0) {
                    // switch
                    T ele = this->buffer_[i];
                    this->buffer_[i] = this->buffer_[j];
                    this->buffer_[j] = ele;
                }
            }
        }
    }

    template <typename X>
    Buffer<X> transform(X (*converter)(T)) {
        Buffer<X> buf;
        for (int i = 0; i < this->length_; i++) {
            buf.add(converter(this->buffer_[i]));
        }
        return buf;
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
    int removeLast(T &last) {
        if (this->length_ == 0) {
            return -1;
        }
        this->length_--;
        last = this->buffer_[this->length_];
        return 1;
    }

    int length() const {
        return this->length_;
    }

    int len() const {
        return this->length_;
    }

    int size() const {
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

    int getLast(T &ele) const {
        if (this->length_ == 0) {
            return -1;
        }
        ele = this->buffer_[this->length_ - 1];
        return 1;
    }

    int getFirst(T &ele) const {
        if (this->length_ == 0) {
            return -1;
        }
        ele = this->buffer_[0];
        return 1;
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

        Array::append(buf2, len2, cap2, DELTA_BUF_CAP, 0, this->buffer_, 0, leftLen);
        Array::append(buf2, len2, cap2, DELTA_BUF_CAP, 0, this->buffer_, idx + 1, rightLen);
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

    bool contains(T ele) {
        return indexOf(ele) >= 0;
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
        Array::copy<T>(this->buffer_, from1, len2, buf);
        return len2;
    }

    void reverse() {
        Array::reverse<T>(this->buffer_, this->length_);
    }

    int write(T *buf, int bLen) {
        int len = Math::min(this->length_, bLen);
        Array::copy(this->buffer_, 0, len, buf, 0);
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

    bool addIfNotExists(T ele) {
        if (this->contains(ele)) {
            return false;
        }
        this->add(ele);
        return true;
    }

    void add(Buffer<T> *buf, int from, int len) {
        this->add(buf->buffer_, from, len);
    }

    void add(const T &element) {
        this->add(&element, 0, 1);
    }

    void add(const T *elements, int length_) {
        add(elements, 0, length_);
    }

    template <typename S>
    void add(const S &element, T (*convert)(const S &)) {
        T ele = convert(element);
        add(&ele, 0, 1);
    }

    void add(const Buffer<T> &buf) {
        add(buf.buffer_, 0, buf.length_);
    }

    void add(const Buffer<T> *buf) {
        add(buf->buffer_, 0, buf->length_);
    }
    void add(const T *elements, const int from, const int len) {

#if A8_BUFFER_DEBUG == 1
        A8_DEBUG4(">>Buffer::add,length_: ", this->length_, ",capacity_:", this->capacity_);
#endif

        Array::add<T>(this->buffer_, this->length_, this->capacity_,
                      DELTA_BUF_CAP, 0,
                      elements, from, len);
#if A8_BUFFER_DEBUG == 1
        A8_DEBUG("<<Buffer<T> *add(const T *elements, const int from, const int len)");
#endif
    }

    Buffer<T> *append(Buffer<T> *buf, int from, int len) {
        this->add(buf, from, len);
        return this;
    }

    Buffer<T> *append(const T &element) {
        add(element);
        return this;
    }

    Buffer<T> *append(const T *elements, int length_) {
        append(elements, length_);
        return this;
    }

    template <typename S>
    Buffer<T> *append(const S &element, T (*convert)(const S &)) {
        add(element, convert);
        return this;
    }

    Buffer<T> *append(const Buffer<T> &buf) {
        add(buf);
        return this;
    }

    Buffer<T> *append(const Buffer<T> *buf) {
        add(buf);
        return this;
    }

    Buffer<T> *append(const T *elements, const int from, const int len) {
        add(elements, from, len);
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
    template <typename C>
    void forEach2(C c, void (*consumer)(C, int, T)) {
        for (int i = 0; i < this->length_; i++) {
            consumer(c, i, this->buffer_[i]);
        }
    }
    Iterator<T> iterator() {
        return Iterator<T>(this);
    }
};

// implementation

} // namespace a8::util
