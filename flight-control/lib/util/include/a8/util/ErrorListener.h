#pragma once
#include "a8/util/String.h"

namespace a8::util {
class ErrorListener {
public:
    virtual void onError(int error, String message);
};

} // namespace a8::util
