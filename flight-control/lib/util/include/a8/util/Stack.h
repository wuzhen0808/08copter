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
class Stack : public Buffer<T> {
public:
    Stack() {
    }
    void push(T ele) {
        this->append(ele);
    }

    T pop(T def) {
        return Buffer<T>::getLast(def);
    }
};

// implementation

} // namespace a8::util
