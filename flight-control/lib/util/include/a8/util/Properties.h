#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Float.h"
#include "a8/util/LineReader.h"
#include "a8/util/Reader.h"
#include "a8/util/String.h"

namespace a8::util {

typedef void (*ValueDelete)(void *);

enum PropertyType {
    zeroType = 0,
    boolType,
    intType,
    longType,
    floatType,
    doubleType,
    stringType,
    externalType,
};

class Entry {
public:
    String name;
    void *value;
    int type;
    Entry(const String &name) {
        this->name = name;
        this->type = zeroType;
        this->value = 0;
    }
    ~Entry() {
        deleteValue();
    }
    void deleteValue() {
        if (type == zeroType || type == externalType || value == 0) {
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
        value = 0;
    }

    void set(int type, void *value) {
        deleteValue();
        this->type = type;
        this->value = value;
    }

    float getFloat(float defValue) {
        if (type != floatType || value == 0) {
            return defValue;
        }
        return *(static_cast<float *>(value));
    }

    int getInt(int defValue) {
        if (type != intType || value == 0) {
            return defValue;
        }
        return *(static_cast<int *>(value));
    }

    String getString(const String &defValue) {
        if (type != stringType || value == 0) {
            return defValue;
        }
        String *sValue = static_cast<String *>(value);

        return *sValue;
    }

    bool getBool(const bool defValue) {
        if (type != boolType || value == 0) {
            return defValue;
        }
        return *(static_cast<bool *>(value));
    }

    void *getExternal(void *defValue) {
        if (type != externalType || value == 0) {
            return defValue;
        }
        return value;
    }
};

class Properties {
    Entry *zeroEntry;

    Buffer<Entry *> *buffer;

    // TODO use a index tree or hash table.
    void set(const String &name, const int type, void *value) {

        Entry *entry = findEntry(name, false);
        if (entry == 0) {
            entry = new Entry(name);
            buffer->append(entry);
        }

        entry->set(type, value);
    }

    Entry *findEntry(const String &name, bool useZeroEntry) {
        for (int i = 0; i < buffer->getLength(); i++) {
            Entry *entry = buffer->get(i);
            if (name == entry->name) {
                return entry;
            }
        }
        if (useZeroEntry) {
            return zeroEntry;
        }
        return 0;
    }

public:
    static void load(Reader &reader, Properties &pts) {
        LineReader lr = LineReader(&reader);
        while (true) {
            String line;
            int ret = lr.readLine(line, false);//
            if (ret == 0) {
                break;
            }
            if (ret < 0) {
                // error
                break;
            }
            if (line.getLength() == 0) {
                //
                continue;
            }
            if (line.getChar(0) == '#') {
                continue;
            }
            int idx = line.indexOf('=');
            if (idx < 0) {
                // ignore this line.
                continue;
            }
            if (idx == 0) {
                // ignore this line.
                continue;
            }

            String name = line.subStr(0, idx).trim();
            String value = line.subStr(idx + 1).trim();
            if (value.getLength() == 0) {
                // ignore this properties. us default value.
                continue;
            }
            String type;
            if (name.endWith("]")) {
                int idx2 = name.lastIndexOf('[');
                if (idx2 > 0) {
                    type = name.subStr(idx2 + 1, name.getLength() - idx2 - 2);
                    name = name.subStr(0, idx2);
                }
            }
            if (type == "float") {
                float v = Float::parse(value);
                pts.set(name, v);
            } else if (type == "int") {
                float v = Float::parse(value);
                pts.set(name, (int)v);
            } else if (type == "long") {
                float v = Float::parse(value);
                pts.set(name, (long)v);
            } else if (type == "double") {
                float v = Float::parse(value);
                pts.set(name, (double)v);
            } else if (type == "string") {
                pts.set(name, value);
            } else if (type == "bool") {
                bool v = (value == "true" || value == "y" || value == "TRUE" || value == "Y" || value == "ON");
                pts.set(name, v);
            } else if (type == "char") {
                char ch = value.getChar(0);
                pts.set(name, ch);
            } else {
                // error process?
            }
        }
    }

    Properties() {
        this->zeroEntry = new Entry("0");
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

    void set(const String &name, int value) {
        this->set(name, intType, new int(value));
    }
    void set(const String &name, char value) {
        this->set(name, intType, new char(value));
    }

    void set(const String &name, long value) {
        this->set(name, longType, new long(value));
    }

    void set(const String &name, const String &value) {
        this->set(name, stringType, new String(value));
    }

    void set(const String &name, const char (&value)[]) {
        String *str = new String(value);
        this->set(name, stringType, str);
    }

    void set(const String &name, const bool value) {
        this->set(name, boolType, new bool(value));
    }

    void set(const String &name, const float value) {
        this->set(name, floatType, new float(value));
    }

    void set(const String &name, const double value) {
        this->set(name, doubleType, new double(value));
    }

    void *getExternal(const String &name, void *defValue) {
        return findEntry(name, true)->getExternal(defValue);
    }

    float getFloat(const String &name, float defValue) {
        return findEntry(name, true)->getFloat(defValue);
    }

    int getInt(const String &name, int defValue) {
        return findEntry(name, true)->getInt(defValue);
    }

    String getString(const String &name, const String &defValue) {
        return findEntry(name, true)->getString(defValue);
    }
    bool getBool(const String &name, const bool defValue) {
        return findEntry(name, true)->getBool(defValue);
    }
};

} // namespace  a8::util
