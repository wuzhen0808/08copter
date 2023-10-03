#pragma once

namespace a8::util::thread {
class Thread {
public:
    Thread() {
    }
    ~Thread() {
    }
    virtual void start() = 0;
};

} // namespace a8::util::thread