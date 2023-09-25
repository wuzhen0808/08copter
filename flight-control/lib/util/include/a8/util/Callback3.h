#pragma once
namespace a8::util {

template <typename T, typename R>
class Callback3 {
public:
    virtual R callback(T context) = 0;
};

} // namespace a8::util