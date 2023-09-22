#pragma once
namespace a8::util {

template <typename T>
class Buffer {
public:
    static const int INC = 100;
    Buffer<T>();
    Buffer<T>(int capacity);
    Buffer<T>(T *buffer, int length);
    Buffer<T>(T *buffer, int length, int capacity);
    ~Buffer();
    int getLength();
    T get(int idx);
    T *getAll();
    int remove(int from);
    Buffer<T> *append(T element);
    Buffer<T> *appendAll(T *elements, int length);

private:
    void init(int cap);
    int capacity;
    int length;
    T *buffer = 0;
    void ensureCapacity(int capacity);
};

// implementation

template <typename T>
Buffer<T>::Buffer() {
    init(INC);
}

template <typename T>
Buffer<T>::Buffer(int capacity) {
    init(capacity);
}

template <typename T>
Buffer<T>::Buffer(T *array, int length) {
    init(length);
    this->appendAll(array, length);
}

template <typename T>
Buffer<T>::Buffer(T *array, int length, int capacity) {
    init(capacity);
    this->appendAll(array, length);
}

template <typename T>
void Buffer<T>::init(int capacity) {
    this->buffer = new T[capacity];
    this->capacity = capacity;
    this->length = 0;
}

template <typename T>
Buffer<T>::~Buffer() {

    delete[] this->buffer;
    this->buffer = 0;

    this->capacity = 0;
    this->length = 0;
}

template <typename T>
void Buffer<T>::ensureCapacity(int capacity) {

    if (this->capacity < capacity) {

        T *tmp1 = this->buffer;
        int cap2 = this->capacity + INC;
        T *tmp2 = new T[cap2];

        for (int i = 0; i < this->length; i++) {
            tmp2[i] = tmp1[i];
        }

        this->buffer = tmp2;
        this->capacity = cap2;
        delete[] tmp1;
    }
}

template <typename T>
T Buffer<T>::get(int idx) {

    if (idx < 0 || idx >= this->length) {
        return 0;
    }
    return this->buffer[idx];
}
template <typename T>
int Buffer<T>::getLength() {
    return this->length;
}

template <typename T>
Buffer<T> *Buffer<T>::append(T element) {
    return appendAll(&element, 1);
}

template <typename T>
Buffer<T> *Buffer<T>::appendAll(T *elements, int length) {
    int length2 = this->length + length;
    ensureCapacity(length2);
    for (int i = 0; i < length; i++) {
        this->buffer[this->length + i] = elements[i];
    }
    this->length = length2;
    return this;
}

template <typename T>
T *Buffer<T>::getAll() {
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
