#pragma once
#include "a8/util/Debug.h"
#include "a8/util/LineReader.h"
#include "a8/util/Logger.h"
#include "a8/util/Output.h"
#include "a8/util/String.h"
#define A8_INPUT_DEBUG (0)
namespace a8::util {
class InputContext {
    int minBoxWidth = 10;
    int maxInfoWidth = 80;
    int currentLineWidth = 0;
    char leftBorder = '>';
    int leftMargin = 1;

public:
    Reader *reader;
    Output *out;
    Logger *logger;
    LineReader *lines;
    InputContext(Reader *reader, Output *out, Logger *logger) {
        this->reader = reader;
        this->lines = new LineReader(reader);
        this->out = out;
        this->logger = logger;
    }
    ~InputContext() {
        delete this->lines;
    }
    String readLine() {
        String line;
        lines->readLine(line, false);
        if (line.endWith('\r')) {
            line = line.subStr(0, line.len() - 1);
        }
        out->println(line);
        return line;
    }

    void println(char ch0, char ch1, int len, char ch2) {
        out->print(ch0);
        for (int i = 0; i < len; i++) {
            out->print(ch1);
        }
        out->print(ch2);
        out->println();
    }
    void startInput(String prompt) {
        println('+', '.', prompt.len(), '.');
        this->currentLineWidth = 0;
        println(prompt);
    }

    void endInput(String prompt) {
        println('.', '.', prompt.len(), '+');
    }
    void println(String prompt) {
        println(0, prompt);
    }
    void println(int intend, String prompt) {
        String line;
        for (int i = 0; i < intend; i++) {
            line << " ";
        }
        line << prompt;
        line << "\n";
        this->doPrint(line);
    }

    void printLeft() {
        out->print(leftBorder);
        // margin
        for (int i = 0; i < leftMargin; i++) {
            out->print(' ');
        }
    }
    void doPrint(String msg) {
        if (this->currentLineWidth == 0) {
            printLeft();
        }
        for (int i = 0; i < msg.len(); i++) {
            char ch = msg.get(i);
            bool autoBreak = false;
            bool userBreak = ch == '\n';
            if (currentLineWidth == maxInfoWidth) {
                autoBreak = true;
            }
            if (autoBreak || userBreak) {
                out->println();
                currentLineWidth = 0;
                printLeft();
            }

            if (!userBreak) {
                out->print(ch);
                currentLineWidth++;
            }
        }
    }

}; // end of InputContext.

template <typename T>
class Input {
protected:
    String prompt;
    T default_;
    T value;

public:
    Input(String prompt, T def) {
        this->prompt = prompt;
        this->default_ = def;
        this->value = def;
    }
    ~Input() {
    }

    void setPrompt(String prompt) {
        this->prompt = prompt;
    }
    String getName() {
        return prompt;
    }
    template <typename F>
    F readNumber(InputContext *ic, String prompt, F def) {
        F fValue = def;
        while (true) {
            String line = ic->readLine();
            if (line.isEmpty()) {
                // use default value.
                break;
            }
            A8_LOG_DEBUG(ic->logger, line);
            F fValue2;

            String debug;
            int ret = Float::parseFloat<T>(line, fValue2, &debug);
            ic->println(debug);
            if (ret < 0) {
                ic->println("cannot parse string(" << line << ",len:" << line.len() << ") as a number, please re-input.");
            } else {
                fValue = fValue2;
                break;
            }
        }
        return fValue;
    }

    T readValue(InputContext *ic) {
        ic->startInput(this->prompt);
        update(ic);
        ic->endInput(this->prompt);
        return this->value;
    }

    virtual void update(InputContext *ic) = 0;
};

template <typename T>
class NumberInput : public Input<T> {
public:
    NumberInput(String prompt, T def) : Input<T>(prompt, def) {
    }

    virtual void update(InputContext *ic) override {
        A8_LOG_DEBUG(ic->logger, ">>readValue.");
        this->value = this->template readNumber<T>(ic, this->prompt, this->default_);
    }
};
class SelectInput : public NumberInput<int> {
    Buffer<String> options;

public:
    SelectInput(String prompt, Buffer<String> options, int def) : NumberInput(prompt, def) {
        this->addOptions(options);
    }

    void addOptions(Buffer<String> options) {
        this->options.append(options);
    }

    void addOption(String option) {
        this->options.append(option);
    }

    void update(InputContext *ic) override {

        for (int i = 0; i < options.len(); i++) {
            ic->println(String() << (default_ == i ? "[*]" : "[ ]") << i << "" << options.get(i));
        }
        ic->println(String() << "Input the number to select the options above.");
        NumberInput<int>::update(ic);
        A8_LOG_DEBUG(ic->logger, String() << "<<readValue:" << value);
    }
};
class BoolInput : public Input<bool> {
    SelectInput *select;

public:
    BoolInput(String prompt, bool def) : Input(prompt, def) {
        Buffer<String> options;
        options.append("Yes");
        options.append("No");
        this->select = new SelectInput(prompt, options, def ? 0 : 1);
    }

    ~BoolInput() {
        delete this->select;
    }
    void update(InputContext *ic) override {
        int selected = select->readValue(ic);
        value = selected == 0 ? true : false;
    }
};

class UnknownInput : public Input<int> {

public:
    UnknownInput(String prompt) : Input(prompt, 0) {
    }
    ~UnknownInput() {
    }

    void update(InputContext *ic) override {
        ic->println("Unknown-Input");
    }
};
} // namespace a8::util
