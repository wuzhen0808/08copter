#pragma once
#include "a8/util/Buffer.h" //TODO remove Buffer dependence.
#include "debug.h"
#define DELTA_STR_CAP (16)

namespace a8 {
namespace util {

class String {

private:
    // Template method must be implemented in header file .

    int length = 0;
    char *text = 0;
    int capacity = 0;

    void init() {
        this->text = 0;
        this->length = 0;
        this->capacity = 0;
    }

    void init(const char *buf, int len) {
        this->init();
        Lang::appendStr(this->text, this->length, this->capacity, DELTA_STR_CAP, buf, 0, len);
    }
    template <typename... Args>
    String &appendFormat(const char format[], Args... args) {
        Lang::appendFormat(text, length, capacity, DELTA_STR_CAP, format, args...);
        return *this;
    }

public:
    
    static String string(const char *str, int len) {
        return String(str, len);
    }

    template <typename... Args>
    static String format(const char format[], Args... args) {
        String ret;
        ret.appendFormat(format, args...);
        // TODO use move constructor?
        return ret;
    }

    // static member funcs
    static int getLength(const char *str);
    static Buffer<String> &strings(int argc, char **argv, Buffer<String> &buf) {

        for (int i = 0; i < argc; i++) {
            String str = String::string(argv[i]);
            if (str.length > 10000) {
                Lang::bug();
            }
            buf.append(str);
        }
        for (int i = 0; i < buf.getLength(); i++) {
            String str = buf.get(i);
            if (str.length > 10000) {
                Lang::bug();
            }
        }
        return buf;
    }

    static String string(const char *str);

    template <typename T>
    Buffer<T> split(const char separator, T (*convert)(String &)) {

        Buffer<T> buffer;
        String str;

        for (int i = 0; i < length; i++) {
            if (text[i] == separator) {

                buffer.append(convert(str));
                str = "";
                continue;
            }
            str.append(text[i]);
        }
        buffer.append(convert(str));

        return buffer;
    }
    // dynamic methods
    String() {
        this->init();
    }

    // String(const char * str), this constructor is not defined.
    // Because for some reason if this method is added then the literal C++ string could not be recognized as String automatically by compiler.
    // So we remove this constructor for now.
    //
    String(const String &str) { // copy constructor
        LOG(">>String::String(const String &str)");

        init(str.text, str.length);
        LOG("<<String::String(const String &str)");
    }

    String(const float f) {
        init();
        Lang::appendFormat(this->text, this->length, this->capacity, DELTA_STR_CAP, "%e", f);
    }

    String(const int i) {
        this->init();
        Lang::appendFormat(this->text, this->length, this->capacity, DELTA_STR_CAP, "%i", i);
    }

    String(const char ch) {
        init(&ch, 1);
    }

    String &operator=(const String &str) { // assign operator
        this->clear();
        Lang::appendStr(this->text, this->length, this->capacity, DELTA_STR_CAP, str.text, 0, str.length);
        return *this;
    }

    String &operator=(const char *buf) {
        this->clear();
        int len = Lang::strLength(buf);
        Lang::appendStr(this->text, this->length, this->capacity, DELTA_STR_CAP, buf, 0, len);
        return *this;
    }

    String &operator=(const char &ch) {
        this->clear();
        this->append(ch);
        return *this;
    }

    String &operator=(const int iValue) {
        this->clear();
        this->append(iValue);
        return *this;
    }

    String &operator=(const float fValue) {
        this->clear();
        this->append(fValue);
        return *this;
    }

    ~String() {
        if (this->text != 0) {
            delete[] this->text;
        }
    }

    String(const char (&buf)[]) {
        int len = Lang::strLength(buf);
        init(buf, len);
    }

    String(const char *buf, int len) {
        LOG(">>String::String(const char *buf, int len)");
        init(buf, len);
        LOG("<<String::String(const char *buf, int len)");
    }

    // other methods
    //  Move constructor does not work for some unknow reason。
    //  Looks like the deconstructor is called unexpectedly when execution split().
    //  The split method take a function pointer as parameter to convert each string split out;
    //  So we remove this constructor for now.
    //  String(const String &&str);
    void clear() {
        this->length = 0;
    }

    bool isEmpty() {
        return this->length = 0;
    }
    /**
     * Return:
     *  0(nullptr)
     *  or a char array end with '\0';
     *
     */
    const char *getText() const {
        return this->text;
    }

    const char *txt() const {
        return this->text;
    }

    int getLength() const;

    int len() const {
        return this->length;
    }

    char getChar(int idx) const {
        return this->text[idx];
    }

    void append(const char *str) {
        append(str, 0, Lang::strLength(str));
    }

    void append(const char ch) {
        append(&ch, 0, 1);
    }

    void append(const String *str) {
        append(str->getText(), 0, str->getLength());
    }

    void append(const char *buf, int len) {
        append(buf, 0, len);
    }
    void append(const String &str) {
        append(str.text, str.length);
    }

    void append(const char *buf, int from, int len) {
        Lang::appendStr(this->text, this->length, this->capacity, DELTA_STR_CAP,
                        buf, from, len);
    }
    void append(const float fValue) {
        Lang::appendFormat(this->text, this->length, this->capacity, DELTA_STR_CAP, "%e", fValue);
    }

    void append(const double fValue) {
        Lang::appendFormat(this->text, this->length, this->capacity, DELTA_STR_CAP, "%e", fValue);
    }

    void append(const int iValue) {
        Lang::appendFormat(this->text, this->length, this->capacity, DELTA_STR_CAP, "%i", iValue);
    }

    void append(const long lValue) {
        Lang::appendFormat(this->text, this->length, this->capacity, DELTA_STR_CAP, "%i", lValue);
    }

    void append(const unsigned iValue) {
        Lang::appendFormat(this->text, this->length, this->capacity, DELTA_STR_CAP, "%i", iValue);
    }

    int lastIndexOf(char ch) {
        for (int i = length - 1; i >= 0; i--) {
            if (this->text[i] == ch) {
                return i;
            }
        }
        return -1;
    }
    int indexOf(char ch) const {
        for (int i = 0; i < length; i++) {
            if (this->text[i] == ch) {
                return i;
            }
        }
        return -1;
    }
    String subStr(int from) const {
        return subStr(from, length - from);
    }
    String subStr(int from, int len) const {
        String ret;
        char *buf = this->text + from;
        ret.append(buf, len);
        return ret;
    }

    String trim() {
        int from = 0;
        for (int i = 0; i < this->length; i++) {
            char ch = this->text[i];
            if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
                from = i;
                continue;
            }
            break;
        }

        if (from > 0) {
            return subStr(from, length - from);
        } else {
            return *this;
        }
    }
    //
    // operators

    String &operator=(const Buffer<char> &iValue);

    String &operator<<(const float fValue);
    String &operator<<(const double fValue);
    String &operator<<(const int fValue);
    String &operator<<(const long fValue);
    String &operator<<(const char *buf);
    String &operator<<(const char buf);
    String &operator<<(const unsigned uValue);
    String &operator<<(const String &str);

    // Other methods
    bool endWith(const char *str);
    Buffer<String> split(const char separator);

    // Friend operators
    friend String operator+(const String &str1, const String &str);
    friend bool operator==(const String &str1, const String &str2);
    friend bool operator==(const String &str1, const int len);
    friend bool operator!=(const String &str1, const int len);
    friend bool operator!=(const String &str1, const String &str2);
};

bool operator==(const String &str1, const String &str2);

} // namespace util

} // namespace a8
