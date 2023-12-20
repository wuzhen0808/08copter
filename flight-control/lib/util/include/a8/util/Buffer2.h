#pragma once
#include "a8/util/Buffer.h"

namespace a8::util {

template <typename T>
class Buffer2 : public Buffer<T> {

    T default_;

public:
    Buffer2(T def) : Buffer<T>() {
    }

    Buffer2(T def, const Buffer<T> &buf) : Buffer<T>(buf) { // copy constructor
    }

    Buffer2(T def, const T *buf, int len) : Buffer<T>(buf, len) { // copy constructor
    }

    T get(int idx) const {
        return Buffer<T>::get(idx, this->default_);
    }
};

// implementation

} // namespace a8::util
