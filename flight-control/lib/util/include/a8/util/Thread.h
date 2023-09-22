#pragma once

namespace a8::util {
class Thread {
public:
    Thread();
    ~Thread();
    virtual void start() = 0;
};
} // namespace a8