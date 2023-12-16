#pragma once
#include "a8/util/Format.h"
#include "a8/util/Lang.h"
#include "a8/util/defines.h"
#include "debug.h"
#define PREFER_INT_WIDTH 1
#define FORMAT_FLOAT32_PRECISION 3
#define FORMAT_FLOAT64_PRECISION 6

namespace a8::util {
/**
 * We do not check the content of the parameters.
 *
 * So the length of the string may wrong if '\0' appended.
 *
 */
class String {

private:
    // Template method must be implemented in header file .

    int length_ = 0;
    char *text_ = 0;
    int capacity_ = 0;

public:
    static String *new_() {
        return new String();
    }

    static String string(const char *str) {
        return String(str, Lang::strLength(str));
    }

    static int hashCode(String str, int fish) {
        int maxLen = 50;
        int len = str.len();
        int sum = 0;
        for (int i = 0; i < len && i < maxLen; i++) {
            sum += (int)str.get(i);
        }
        return sum % fish;
    }

    // dynamic methods
    String() {
    }

    // String(const char * str), this constructor is not defined.
    // Because for some reason if this method is added then the literal C++ string could not be recognized as String automatically by compiler.
    // So we remove this constructor for now.
    //
    String(const String &str) { // copy constructor
        append(str.text_, 0, str.length_);
    }

    String(String &&str) { // move constructor
        this->text_ = str.text_;
        this->length_ = str.length_;
        this->capacity_ = str.capacity_;
        str.text_ = 0;
        str.length_ = 0;
        str.capacity_ = 0;
    }

    String(const float f) {
        append(f);
    }

    String(const int i) {
        append(i);
    }

    String(const char ch) {
        append(&ch, 0, 1);
    }

    char get(int i) {
        return charAt(i);
    }

    String &operator=(const String &str) { // assign operator
        this->clear();
        append(str.text_, 0, str.length_);
        return *this;
    }

    String &operator=(const char *buf) {
        this->clear();
        append(buf, 0, Lang::strLength(buf));
        return *this;
    }

    String &operator=(const char &ch) {
        this->clear();
        this->append(&ch, 0, 1);
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
        if (this->text_ != 0) {
            delete[] this->text_;
        }
    }

    String(const char *buf) {
        int len = Lang::strLength(buf);
        append(buf, 0, len);
    }

    String(const char *buf, int len) {
        append(buf, 0, len);
    }

    // other methods
    //  Move constructor does not work for some unknow reason。
    //  Looks like the deconstructor is called unexpectedly when execution split().
    //  The split method take a function pointer as parameter to convert each string split out;
    //  So we remove this constructor for now.
    //  String(const String &&str);
    void clear() {
        this->length_ = 0;
    }

    bool isEmpty() const {
        return this->length_ == 0;
    }
    /**
     * Return:
     *  0(nullptr)
     *  or a char array end with '\0';
     *
     */
    char *text() const {
        return this->text_;
    }

    int length() const {
        return this->length_;
    }
    int size() const {
        return length_ + 1;
    }
    int len() const {
        return this->length_;
    }

    char charAt(int idx) const {
        return this->text_[idx];
    }
    void setLength(char fill, int len) {
        while (this->length_ < len) {
            append(fill);
        }
    }
    void append(const char *str) {
        append(str, 0, Lang::strLength(str));
    }

    void append(const char ch) {
        append(&ch, 0, 1);
    }

    void append(const char *buf, int len) {
        append(buf, 0, len);
    }

    void append(const String &str) {
        append(str.text_, 0, str.length_);
    }

    void append(const char *buf, int from, int len) {
        Lang::appendStr(this->text_, this->length_, this->capacity_, DELTA_STR_CAP, buf, from, len);
    }

    void append(const float fValue) {
        append<float>(0, ' ', fValue, FORMAT_FLOAT32_PRECISION);
    }

    void append(const double fValue) {
        append<double>(0, ' ', fValue, FORMAT_FLOAT64_PRECISION);
    }

    void append(const int iValue, const int width, const char fill) {
        append<int>(width, fill, iValue, 0);
    }

    template <typename T>
    void append(int width, char fill, T value, int precision) {
        Format::append<T>(this->text_, this->length_, this->capacity_, DELTA_STR_CAP,
                          width, fill, value, precision, true);
    }

    void append(const int iValue) {
        append<int>(0, ' ', iValue, 0);
    }

    void append(const bool bValue) {
        append(bValue ? "true" : "false");
    }

    void append(const long lValue) {
        append<int>(0, ' ', lValue, 0);
    }

    void append(const long long lValue) {
        append<int>(0, ' ', lValue, 0);
    }

    void append(const unsigned iValue) {
        append<unsigned>(0, ' ', iValue, true);
    }

    int lastIndexOf(char ch) const {
        for (int i = this->length_ - 1; i >= 0; i--) {
            if (this->text_[i] == ch) {
                return i;
            }
        }
        return -1;
    }
    int indexOf(char ch) const {
        for (int i = 0; i < this->length_; i++) {
            if (this->text_[i] == ch) {
                return i;
            }
        }
        return -1;
    }
    String subStr(int from) const {
        int len;
        if (from >= 0) {
            len = this->length_ - from;
        } else {
            len = -from;
            from = length_ + from;
        }
        if (len < 0 || from >= this->length_) {
            String str;
            return str;
        }

        return subStr(from, len);
    }

    String subStr(int from, int len) const {
        String ret;
        char *buf;
        if (from >= 0) {
            buf = this->text_ + from;
        } else {
            buf = this->text_ + this->length_ + from;
        }
        if (len < 0) {
            len = this->length_ + len;
        }
        ret.append(buf, len);
        return ret;
    }

    String trim() {
        int from = 0;
        for (int i = 0; i < this->length_; i++) {
            char ch = this->text_[i];
            if (ch == SPACE || ch == '\t' || ch == '\r' || ch == NEW_LINE) {
                from = i;
                continue;
            }
            break;
        }

        if (from > 0) {
            return subStr(from, this->length_ - from);
        } else {
            return *this;
        }
    }

    bool equalsIgnoreCase(const String str1) const {
        int len1 = str1.length();
        int len2 = this->length();
        if (len1 != len2) {
            return false;
        }
        for (int i = 0; i < len1; i++) {
            char c1 = str1.charAt(i);
            char c2 = this->charAt(i);

            if (!Lang::equalsIgnoreCase(c1, c2)) {
                return false;
            }
        }
        return true;
    }

    //
    // operators

    // operators

    String &operator<<(const float fValue) {
        append(fValue);
        return *this;
    }

    int operator%(int iValue) {
        return hashCode(*this, iValue);
    }

    String &operator<<(const bool bValue) {
        append(bValue);
        return *this;
    }
    String &operator<<(const double fValue) {
        append(fValue);
        return *this;
    }
    String &operator<<(const int fValue) {
        append(fValue);
        return *this;
    }
    String &operator<<(const long fValue) {
        append(fValue);
        return *this;
    }

    String &operator<<(const long long fValue) {
        append(fValue);
        return *this;
    }

    String &operator<<(const unsigned fValue) {
        append(fValue);
        return *this;
    }

    String &operator<<(const char *buf) {
        append(buf);
        return *this;
    }

    String &operator<<(const char ch) {
        append(ch);
        return *this;
    }

    String &operator<<(const String &str) {
        append(str);
        return *this;
    }

    char operator[](int idx) const {
        if (idx < 0 || idx > this->length_) {
            Lang::illegalArgument("index out of bound.");
        }
        return this->text_[idx];
    }

    bool startWith(const String &str, int from2) const {
        return isMatch(0, str.text_, from2, str.length_ - from2);
    }

    bool startWith(const String &str) const {
        return isMatch(0, str.text_, 0, str.length_);
    }

    bool startWith(const char ch) const {
        return isMatch(0, &ch, 0, 1);
    }

    bool endWith(const String &str) const {
        return isMatch(this->length_ - str.length_, str.text_, 0, str.length_);
    }

    bool endWith(const char ch) const {
        return isMatch(this->length_ - 1, &ch, 0, 1);
    }

    // Other methods
    bool endWith(const char *str, int len) const {
        return isMatch(this->length_ - len, str, 0, len);
    }

    int indexOf(int from, const char *str, int from2, int len2) const {
        for (int i = from; i < this->length_ - len2; i++) {
            if (isMatch(i, str, from2, len2)) {
                return i;
            }
        }
        return -1;
    }
    // Other methods
    bool isMatch(int from, const char *str, int from2, int len2) const {
        if (this->length_ - from < len2) {
            return false;
        }

        for (int i = 0; i < len2; i++) {
            if (this->text_[from + i] != str[from2 + i]) {
                return false;
            }
        }
        return true;
    }

    // Friend operators

    friend String operator<<(const char *str, String &str2) {
        String str_;
        str_.append(str);
        str_.append(str2);
        return str_;
    }

    friend String operator+(String const &str1, String const &str2) {
        String ret;
        ret.append(str1);
        ret.append(str2);
        return ret;
    }

    friend bool operator==(String const &str1, String const &str2) {
        return !operator!=(str1, str2);
    }
    friend bool operator==(String const &str1, const int len) {
        return str1.length() == len;
    }
    friend bool operator!=(String const &str1, const int len) {
        return str1.length() != len;
    }

    friend bool operator!=(String const &str1, String const &str2) {
        int len1 = str1.length();
        int len2 = str2.length();
        if (len1 != len2) {
            return true;
        }
        for (int i = 0; i < len1; i++) {
            if (str1.charAt(i) != str2.charAt(i)) {
                return true;
            }
        }
        return false;
    }
};

} // namespace a8::util
