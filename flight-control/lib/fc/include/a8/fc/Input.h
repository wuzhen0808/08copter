#pragma once
#include "a8/fc/Config.h"
#include "a8/fc/Pilot.h"
#include "a8/util.h"
#include "a8/util/sched.h"

namespace a8::fc {
using namespace a8::util;

class Input {
protected:
    LineReader *reader;
    String prompt;
    void *default_;
    Output *out;
    void (*releaseDefault)(void *);

public:
    Input(LineReader *reader, Output *out, String prompt, void *def, void (*releaseDef)(void *)) {
        this->reader = reader;
        this->out = out;
        this->default_ = def;
        this->prompt = prompt;
        this->releaseDefault = releaseDef;
    }
    ~Input() {
        this->releaseDefault(this->default_);
    }
    void setPrompt(String prompt) {
        this->prompt = prompt;
    }
    String getName() {
        return prompt;
    }
    void log(String msg) {
        out->println(msg);
    }
    String readLine() {
        String line;
        reader->readLine(line, false);
        if (line.endWith('\r')) {
            line = line.subStr(0, line.len() - 1);
        }
        out->println(line);
        return line;
    }
    template <typename T>
    T readNumber(String prompt, T def) {
        T fValue = def;
        while (true) {
            out->println(prompt);
            String line = readLine();
            if (line.isEmpty()) {
                // use default value.
                break;
            }
            T fValue2;
            int ret = Float::parseFloat<T>(line, fValue2, 0);
            if (ret < 0) {
                out->println("cannot parse string(" << line << ",len:" << line.len() << ") as a number, please re-input.");
            } else {
                fValue = fValue2;
                break;
            }
        }
        return fValue;
    }
    virtual void readValue(Logger *logger, void *context, void (*consumer)(void *, void *)) = 0;
};

template <typename T>
class NumberInput : public Input {
public:
    NumberInput(LineReader *reader, Output *out, String prompt, T def) : Input(reader,
                                                                               out,
                                                                               prompt,
                                                                               new T(def),
                                                                               [](void *def) {
                                                                                   delete static_cast<T *>(def);
                                                                               }) {
    }
    T getDefault() {
        return *static_cast<T *>(this->default_);
    }

    void readValue(Logger *logger, void *context, void (*consumer)(void *, void *)) override {
        logger->debug(">>readValue.");
        T def = getDefault();
        T fValue = this->readNumber<T>(this->prompt, def);
        logger->debug("..calling consumer");
        consumer(context, &fValue);
        logger->debug(String() << "<<readValue.");
    }
};
class SelectInput : public NumberInput<int> {
    Buffer<String> options;

public:
    SelectInput(LineReader *reader, Output *out, String prompt, int def) : NumberInput(reader, out, prompt, def) {
    }
    void addOption(String option) {
        options.append(option);
    }

    void readValue(Logger *logger, void *context, void (*consumer)(void *, void *)) override {

        int def = *static_cast<int *>(this->default_);
        for (int i = 0; i < options.len(); i++) {
            out->println(String() << (def == i ? "[*]" : "[ ]") << i << "" << options.get(i));
        }
        out->println(String() << "Input the number to select the options above.");
        int value = this->readNumber<int>(this->prompt, def);
        logger->debug(String() << "select Input,calling.. consumer..readNumber:" << value);
        consumer(context, &value);
        logger->debug(String() << "<<readValue:" << value);
    }
};
class BoolInput : public SelectInput {

public:
    BoolInput(LineReader *reader, Output *out, String prompt, bool def) : SelectInput(reader, out, prompt, def ? 0 : 1) {
        addOption("Yes");
        addOption("No");
    }
    ~BoolInput() {
    }
    void readValue(Logger *logger, void *context, void (*consumer)(void *, void *)) override {
        struct Params {
            void *context;
            void (*consumer)(void *, void *);
        } p;
        SelectInput::readValue(logger, &p, [](void *pp, void *value) {
            Params *ppp = static_cast<Params *>(pp);
            int selected = *static_cast<int *>(value);
            bool bValue = selected == 0 ? true : false;
            ppp->consumer(ppp->context, &bValue);
        });
    }
};

class UnknownInput : public Input {

public:
    UnknownInput(LineReader *reader, Output *out, String prompt) : Input(reader, out, prompt, 0, [](void *) {}) {
    }
    ~UnknownInput() {
    }

    void readValue(Logger *logger, void *context, void (*consumer)(void *, void *)) override {
        out->println("Unknown-Input");
    }
};
} // namespace a8::fc
