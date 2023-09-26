#pragma once
#include "a8/util/Component.h"

namespace a8::util {
template <class T>
class WrapperComponent : public Component {
public:
    T *wrapped;
    WrapperComponent(T *wrapped) : Component("wrp") {
        this->wrapped = wrapped;
    }
};

} // namespace a8::util
