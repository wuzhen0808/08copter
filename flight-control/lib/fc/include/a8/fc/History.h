#pragma once
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
class History {
public:
    Buffer<String> messages;
    void add(int depth, String msg) {
        // A8_DEBUG4(">>History::add,depth:", depth, ",msg:", msg.text());
        this->messages.append(String() << StringUtil::space(depth) << msg);
        // A8_DEBUG("<<add.2");
    }
    void reset() {
        messages.clear();
    }
    void print(Output *out) {
        if (messages.isEmpty()) {
            out->println("No history.");
        }
        for (int i = 0; i < messages.len(); i++) {
            String msg = messages.get(i);
            out->println(msg);
        }
    }
};

} // namespace a8::fc