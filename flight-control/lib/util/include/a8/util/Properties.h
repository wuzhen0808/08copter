#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/String.h"

namespace a8::util {

typedef void (*ValueDelete)(void *);

enum PropertyType {
    boolType = 0,
    intType,
    floatType,
    stringType
};

class Entry {
public:
    String name;
    void *value;
    int type;
    Entry(const String &name, void *value, int type) {
        this->name = name;
        this->value = value;
        this->type = type;
    }
    ~Entry() {
        deleteValue();
    }
    void deleteValue() {
        if (this->value == 0) {
            return;
        }
        switch (this->type) {
        case boolType:
            delete static_cast<bool *>(value);
            break;
        case intType:
            delete static_cast<int *>(value);
            break;
        case floatType:
            delete static_cast<float *>(value);
            break;
        case stringType:
            delete static_cast<String *>(value);
            break;
        default:;
            // exception throw?
        }
    }
    void set(void *value) {
        deleteValue();
        this->value = value;
    }
};

class Properties {

    Buffer<Entry *> *buffer;
    // TODO use a index tree or hash table.
    void set(const String &name, const int type, void *value) {

        Entry *entry = findEntry(name);
        if (entry == 0) {
            entry = new Entry(name, value, type);
            buffer->append(entry);
            return;
        }
        entry->set(value);
    }
    Entry *findEntry(const String &name) {
        for (int i = 0; i < buffer->getLength(); i++) {
            Entry *entry = buffer->get(i);
            if (name == entry->name) {
                return entry;
            }
        }
        return 0;
    }
    void *get(const String &name, int type) {

        Entry *entry = findEntry(name);
        if (entry == 0) {
            return 0;
        }
        return entry->value;
    }

public:
    Properties() {
        this->buffer = new Buffer<Entry *>();
    }

    ~Properties() {

        for (int j = 0; j < buffer->getLength(); j++) {
            Entry *entry = buffer->get(j);
            delete entry;
        }

        delete buffer;
        buffer = 0;
    }

    void set(const String &name, float value) {
        this->set(name, floatType, new float(value));
    }

    float getFloat(const String &name, float defValue) {
        void *value = this->get(name, floatType);
        if (value == 0) {
            return defValue;
        }
        return *(static_cast<float *>(value));
    }

    void set(const String &name, int value) {
        this->set(name, floatType, new int(value));
    }

    int getInt(const String &name, int defValue) {
        void *value = this->get(name, floatType);
        if (value == 0) {
            return defValue;
        }
        return *(static_cast<int *>(value));
    }

    void set(const String &name, const String &value) {
        this->set(name, floatType, new String(value));
    }

    String getString(const String &name, const String &defValue) {
        void *value = this->get(name, floatType);
        if (value == 0) {
            return defValue;
        }
        return *(static_cast<String *>(value));
    }

    void set(const String &name, const bool value) {
        this->set(name, boolType, new bool(value));
    }

    bool getBool(const String &name, const bool defValue) {
        void *value = this->get(name, floatType);
        if (value == 0) {
            return defValue;
        }
        return *(static_cast<bool *>(value));
    }
};

} // namespace  a8::util
