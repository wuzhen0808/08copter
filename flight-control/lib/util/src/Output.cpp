
#include "a8/util/Output.h"
#include "a8/util/Format.hpp"

using namespace a8::util;

namespace a8::util {

void Output::print(int ivalue){
    String fmt("123");
    String str = Format::format(fmt, ivalue);
    this->print(str);
}

void Output::println(String str) {
    this->print(str);
    String ln("\n");
    this->print(ln);
}
} // namespace a8::util