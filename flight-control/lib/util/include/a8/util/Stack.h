#pragma once
#include "a8/util/Assert.h"
#include "a8/util/Buffer.h"
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

    int peek(T &ele) {        
        return this->getLast(ele);
    }

    int pop() {
        T ele;
        return this->pop(ele);
    }

    int pop(T &ele) {
        return this->removeLast(ele);
    }
};

// implementation

} // namespace a8::util
