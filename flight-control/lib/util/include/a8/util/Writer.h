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
    void write(const char *str) {
        writeUntil(str, Util::END_OF_STR);
    }

    void writeUntil(const char *buf, char til) {
        for (int i = 0;; i++) {
            if (buf[i] == til) {
                break;
            }
            write(buf, 1);
        }
    }
    
    virtual void write(const char *buf, int bufLen) = 0;
};

} // namespace a8::util