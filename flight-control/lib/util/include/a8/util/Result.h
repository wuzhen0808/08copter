#pragma once
#include "a8/util/String.h"

namespace a8::util {

using a8::util::String;

class Result {
public:
    int error;
    String message;
    void *data;
    Result(int error) {
        this->error = error;
    }

    Result(bool success) {
        init(success);
    }

    Result(int error, String message) {
        this->error = error;
        this->message = message;
    }

    String toString() {
        return String::format("error:%i, message:%s", error, message.getText());
    }

    Result &operator=(String message) {
        this->error = 1;
        this->message = message;
        return *this;
    }
    void init(bool success) {
        if (success) {
            this->error = 0;
            this->message = "Success.";
        } else {
            this->error = 1;
            this->message = "Unkown Error.";
        }
    }
    Result &operator=(bool success) {
        init(success);
        return *this;
    }

    Result &operator=(int error) {
        this->error = error;
        return *this;
    }

    ~Result() {
    }
};
} // namespace a8::util