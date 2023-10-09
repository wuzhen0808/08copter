#pragma once

namespace a8::util {
template <typename T>
class MovePtr {

public:
    T *ptr;
    MovePtr() {
        ptr = 0;
    }
    MovePtr(T *ptr) {
        this->ptr = ptr;
    }

    MovePtr(MovePtr<T> &ref) {
        this->ptr = ref->ptr;
        ref->ptr = 0;
    }

    ~MovePtr() {
        if (ptr != 0) {
            delete ptr;
        }
    }

    void operator=(T *ptr) {
        if (this->ptr != 0) {
            delete this->ptr;
        }
        this->ptr = ptr;
    }

    friend bool operator==(MovePtr<T> &ref, int zero) {
        if (zero == 0) {
            return ref.ptr == 0;
        }
        return false;
    }
};
} // namespace a8::util