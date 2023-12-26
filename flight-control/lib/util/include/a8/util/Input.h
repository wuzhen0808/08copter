#pragma once
#include "a8/util/Debug.h"
#include "a8/util/Keys.h"
#include "a8/util/LineReader.h"
#include "a8/util/Logger.h"
#include "a8/util/Output.h"
#include "a8/util/String.h"
#define A8_INPUT_DEBUG (0)
namespace a8::util {
class InputContext {
private:
    InputContext(const InputContext &cc);
    InputContext &operator=(const InputContext &cc);

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

    int readChar(char &ch, Result &res) {
        return lines->read(&ch, 1, res);
    }

    template <typename C>
    int readChar(C c, bool (*handle)(C, char)) {
        int len = 0;
        while (true) {
            char ch;
            Result res;
            int ret = this->readChar(ch, res);
            if (ret <= 0) {
                return ret;
            }
            bool done = handle(c, ch);
            len++;
            if (done) {
                break;
            }
        }
        return len;
    }
    int readOneOfTheChar(Buffer<char> chs, char &ch, Result &res) {
        while (true) {
            char ch2;
            int ret = this->readChar(ch2, res);
            if (ret < 0) {
                return ret;
            }
            if (chs.contains(ch2)) {
                ch = ch2;
                break;
            }
        }
        return 1;
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
template <typename C>
class SelectInput : public NumberInput<int> {

public:
    using optionsF = String (*)(C, int);
    using releaseContextF = void (*)(C);

protected:
    C context;
    optionsF options_;
    int focused = 0;
    int len;
    releaseContextF releaseContext;

public:
    SelectInput(String prompt, C c, int len, optionsF options, int def) : NumberInput(prompt, def) {
        this->init(c, len, options, 0);
    }

    SelectInput(String prompt, C c, releaseContextF rcf, int len, optionsF options, int def) : NumberInput(prompt, def) {
        this->init(c, len, options, rcf);
    }

    void init(C c, int len, optionsF options, releaseContextF rcf) {
        this->context = c;
        this->len = len;
        this->options_ = options;
        this->releaseContext = rcf;
    }

    ~SelectInput() {
        if (this->releaseContext != 0) {
            this->releaseContext(this->context);
        }
    }

    void print(InputContext *ic) {
        for (int i = 0; i < this->len; i++) {

            ic->println(String() << " " << (focused == i ? " o->" : "    ") //
                                 << i                                       //
                                 << (value == i ? "[*]" : "[ ]")            //
                                 << this->options_(context, i));
        }
        ic->println(String() << "Press up/down and enter to select the options above.");
    }
    void update(InputContext *ic) override {
        this->focused = this->value;
        struct Params {
            SelectInput *this_;
            bool done = false;
        } p;
        p.this_ = this;
        while (!p.done) {
            this->print(ic);
            ic->readChar<Params *>(&p, [](Params *p, char ch) {
                return p->this_->onKey(ch, p->done);
            });
        }
        A8_LOG_DEBUG(ic->logger, String() << "<<readValue:" << value);
    }

    bool onKey(char ch, bool &done) {

        switch (ch) {
        case Keys::upKey: {
            focused--;
            if (focused < 0) {
                focused += this->len;
            }
            this->focused %= this->len;

        } break;
        case Keys::downKey: {
            focused++;
            this->focused %= this->len;
        } break;
        case Keys::enterKey: {
            this->value = this->focused;
            done = true;
        } break;
        default:
            return false;
        }
        return true;
    }
};
class BoolInput : public Input<bool> {
    SelectInput<BoolInput *> *select;

public:
    BoolInput(String prompt, bool def) : Input(prompt, def) {
        String (*options)(BoolInput *, int) = [](BoolInput *this_, int idx) {
            return idx == 0 ? String("Yes") : String("No");
        };
        this->select = new SelectInput<BoolInput *>(prompt, this, 2, options, def ? 0 : 1);
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
