#pragma once
namespace a8::util {
/**
 */
class Writer {
protected:
public:
    Writer() {
    }

    ~Writer() {
    }

    virtual void write(const char *buf, int bufLen) = 0;

};

} // namespace a8::util