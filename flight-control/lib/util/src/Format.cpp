#include "a8/util/Format.h"
namespace a8::util {

const Format::Float *Format::defaultFloatFormat = new AutoOffsetFloat(8, 2);
const Format::Float *Format::defaultDoubleFormat = new AutoOffsetFloat(16, 3);
const Format::Float *Format::defaultLongDoubleFormat = new AutoOffsetFloat(18, 6);

} // namespace a8::util