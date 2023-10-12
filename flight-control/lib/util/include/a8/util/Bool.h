#pragma once
#include "a8/util/Math.h"
#include "a8/util/String.h"
#include "a8/util/StringUtil.h"
namespace a8::util {
using namespace a8::util;
class Bool {
public:
    static bool parse(const String &bStr) {        
        return bStr.equalsIgnoreCase("true") || bStr.equalsIgnoreCase("y") || bStr.equalsIgnoreCase("yes");
    }
};

} // namespace a8::util
