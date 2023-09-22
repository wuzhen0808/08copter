#pragma once
namespace a8::util {

template <typename T>
class Callback2 {
public:
    virtual void callback(T context) = 0;
};

} // namespace a8::util