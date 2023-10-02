#pragma once
#include "a8/util/String.h"

namespace a8::util {

using a8::util::String;

class Result {

public:
    String errorMessage;
    String successMessage;
};
} // namespace a8::util