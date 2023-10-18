#pragma once
#include "a8/util/comp/Component.h"

using namespace a8::util;
using namespace a8::util::schedule;

namespace a8::util::comp {
template <class T>
class WrapperComponent : public Component {
public:
    T *wrapped;
    WrapperComponent(T *wrapped) : Component("wrp") {
        this->wrapped = wrapped;
    }
};

} // namespace a8::util::comp
