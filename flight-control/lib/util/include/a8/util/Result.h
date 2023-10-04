#pragma once
#include "a8/util/String.h"

namespace a8::util {

using a8::util::String;

class Result {

public:
    String errorMessage;
    String successMessage;

    friend String &operator<<(String &str, Result &rst) {
        return str << rst.errorMessage;
    }
};
} // namespace a8::util