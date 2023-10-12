#pragma once
#include "a8/util/String.h"

namespace a8::util {

using a8::util::String;
/**
 * Because the result is used as the reference parameter and my singleton at the thread context.
 *
 * Do not append errorMessage or successMessage only when the result is definitely the final result.
 *
 * Otherwise there will be a mem leak.
 *
 */
class Result {

public:
    String errorMessage;

    int reset() {
        int ret = this->errorMessage.len();
        this->errorMessage.clear();
        return ret;
    }

    Result &operator<<(const String &str) {
        this->errorMessage << str;
        return *this;
    }

    friend String &operator<<(String &str, const Result &rst) {
        return str << rst.errorMessage;
    }
};
} // namespace a8::util