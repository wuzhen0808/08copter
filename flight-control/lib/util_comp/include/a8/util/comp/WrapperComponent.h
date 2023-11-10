#pragma once
#include "a8/util/comp/Component.h"

namespace a8::util::comp {
using namespace a8::util;
using namespace a8::util::schedule;

template <class T>
class WrapperComponent : public Component {
public:
    T *wrapped;
    WrapperComponent(T *wrapped) : Component("wrp") {
        this->wrapped = wrapped;
    }
};

} // namespace a8::util::comp
