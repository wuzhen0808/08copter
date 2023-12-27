#pragma once
#include "a8/util/Buffer.h"

namespace a8::util {

class BufferUtil {

public:
    template <typename T>
    static T *get(Buffer<T *> *buffer, int idx, T *def) {
        return buffer->get(idx, def);
    }

    template <typename T>
    static T *get(Buffer<T *> *buffer, int idx) {
        return buffer->get(idx, 0);
    }

    template <typename T>
    static T *get(Buffer<T *> &buffer, int idx, T *def) {
        return buffer.get(idx, def);
    }

    template <typename T>
    static T *get(Buffer<T *> &buffer, int idx) {
        return buffer.get(idx, 0);
    }
};

// implementation

} // namespace a8::util
