#pragma once
#include "a8/util/String.h"
namespace a8::util {

class Assert {
private:
    void emptyMethod() const {
    }

public:

    static void illegalArgument(const String msg) {
        static_cast<Assert *>(0)->emptyMethod();
    }
};

} // namespace a8::util
