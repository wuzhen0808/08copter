#ifndef string__
#define string__
#include "a8/util/String.hpp"

namespace a8 {
namespace util {
using namespace a8::util;
class Format {
public:
    static String format(String format, int value);
};

} // namespace util

} // namespace a8

#endif