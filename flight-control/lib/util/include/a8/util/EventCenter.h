#pragma once
#include "a8/util/Buffer.h"

namespace a8::util {

class EventCenter {

    struct Entry {
        void (*eventHandle)(void *, void *, void *); // eventData, context, handle
        void *handle;
        void *context;
    };

    // fields
    Buffer<Buffer<Entry *> *> *entries;
    int nextType_ = 1;

public:
    EventCenter() {
        this->entries = new Buffer<Buffer<Entry *> *>();
    }
    int nextType() {
        return nextType_++;
    }

    // methods
    void notifyEvent(int type, void *data) {
        Buffer<Entry *> *buf = this->get(type, false);
        if (buf == 0) {
            return;
        }
        for (int i = 0; i < buf->len(); i++) {
            Entry *en = buf->get(i);
            en->eventHandle(data, en->context, en->handle);
        }
    }

    Buffer<Entry *> *get(int type, bool create) {
        Buffer<Entry *> *buf = 0;
        if (type < entries->len()) {
            buf = entries->get(type);
        }
        if (buf == 0 && create) {
            buf = new Buffer<Entry *>();
            entries->set(type, 0, buf);
        }
        return buf;
    }

    template <typename T, typename C>
    void add(int type, C context, void (*handle)(T, C)) {
        using handleType = void (*)(T, C);
        Entry *en = new Entry();
        en->context = context;
        en->handle = reinterpret_cast<void *>(handle);
        en->eventHandle = [](void *eventDataV, void *contextV, void *handleV) {
            T data = Lang::cast<T>(eventDataV);
            C context = Lang::cast<C>(contextV);
            handleType handle = reinterpret_cast<handleType>(handleV);
            handle(data, context);
        };

        this->get(type, true)->append(en);
    }
};
} // namespace a8::util