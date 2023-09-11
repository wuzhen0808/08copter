#pragma once

namespace a8 {
namespace core {
class Thread {
public:
    Thread();
    ~Thread();
    virtual void start() = 0;
};
} // namespace core
} // namespace a8