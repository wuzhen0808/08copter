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
    String successMessage;

    friend String &operator<<(String &str, Result &rst) {
        return str << rst.errorMessage;
    }
};
} // namespace a8::util