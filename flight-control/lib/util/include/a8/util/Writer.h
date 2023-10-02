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
    void write(const char ch) {
        write(&ch, 1);
    }
    void write(const char *str) {
        writeUntil(str, Lang::END_OF_STR);
    }

    void writeUntil(const char *buf, char til) {
        for (int i = 0;; i++) {
            if (buf[i] == til) {
                break;
            }
            write(buf, 1);
        }
    }
    void write(String str) {
        write(str.getText(), str.getLength());
    }

    virtual void write(const char *buf, int bufLen) = 0;
    Writer &operator<<(const char ch) {
        write(ch);
        return *this;
    }
    Writer &operator<<(const char *str) {
        write(str);
        return *this;
    }
    Writer &operator<<(const String str) {
        write(str);
        return *this;
    }
};

} // namespace a8::util