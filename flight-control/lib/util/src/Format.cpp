#include "a8/util/Format.h"
namespace a8::util {

const Format::Float *Format::defaultFloatFormat = new AutoOffsetFloat(8, 2);
const Format::Float *Format::defaultDoubleFormat = new AutoOffsetFloat(15, 3);
const Format::Float *Format::defaultLongDoubleFormat = new AutoOffsetFloat(16, 6);
const Format::Float *Format::defaultNoTailFloatFormat = new AutoOffsetFloat(8, 0);
const Format::Float *Format::defaultNoTailDoubleFormat = new AutoOffsetFloat(15, 0);
const Format::Float *Format::defaultNoTailLongDoubleFormat = new AutoOffsetFloat(16, 0);
} // namespace a8::util