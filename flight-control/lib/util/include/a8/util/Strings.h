#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Lang.h"

namespace a8::util {

class Strings {
private:
    char **content_ = 0;
    int len_ = 0;

public:
    Strings() {
        
    }
    Strings(Buffer<String> buf) {
        this->operator=(buf);
    }

    Strings(Strings &strs) {
        this->operator=(strs);
    }

    Strings(Strings &&a2) {
        move(a2);
    }

    void move(Strings &a2) {
        this->content_ = a2.content_;
        this->len_ = a2.len_;
        a2.content_ = 0;
        a2.len_ = 0;
    }

    ~Strings() {
        Lang::free(this->content_, this->len_, true);
    }

    char **content() {
        return this->content_;
    }

    int len() {
        return len_;
    }

    Strings &operator=(Strings &strs) {
        char **content2 = new char *[strs.len_] { 0 };
        for (int i = 0; i < strs.len_; i++) {
            Array::appendStr(content2[i], strs.content_[i]);
        }
        Lang::replace(this->content_, this->len_, content2, strs.len_, true);
        return *this;
    }

    // static member funcs
    Strings &operator=(Buffer<String> &buf) {

        char **content2 = new char *[buf.length()] {};
        for (int i = 0; i < buf.len(); i++) {
            String str = buf[i];
            Array::appendStr(content2[i], str.text(), 0, str.len());
        }
        Lang::replace(this->content_, this->len_, content2, buf.len(), true);
        return *this;
    }
};

} // namespace a8::util
