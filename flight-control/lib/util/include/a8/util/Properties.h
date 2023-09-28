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
    charType,
    externalType = 99,
};

class Entry {
public:
    String name;
    void *value;
    int type;
    bool isArray;
    String line;
    Entry(const String &name) {
        this->name = name;
        this->type = zeroType;
        this->value = 0;
    }
    ~Entry() {
        deleteValue();
    }
    String getLine() {
        if (this->line == "") {
            return this->name + "=?";
        }
        return line;
    }

    void deleteValue() {
        if (type == zeroType || type == externalType || value == 0) {
            return;
        }
        switch (this->type) {
        case boolType:
            delete static_cast<Buffer<bool> *>(value);
            break;
        case intType:
            delete static_cast<Buffer<int> *>(value);
            break;
        case floatType:
            delete static_cast<Buffer<float> *>(value);
            break;
        case stringType:
            delete static_cast<Buffer<String> *>(value);
            break;
        case longType:
            delete static_cast<Buffer<long> *>(value);
            break;
        case charType:
            delete static_cast<Buffer<char> *>(value);
            break;

        default:;
            // exception throw?
        }
        value = 0;
    }

    void set(int type, bool isArray, void *value) {
        deleteValue();
        this->type = type;
        this->isArray = isArray;
        this->value = value;
    }

    Buffer<float> *getFloatArray() {
        if (type != floatType) {
            return 0;
        }
        return static_cast<Buffer<float> *>(value);
    }

    Buffer<double> *getDoubleArray() {
        if (type != doubleType) {
            return 0;
        }
        return static_cast<Buffer<double> *>(value);
    }
    Buffer<int> *getIntArray() {
        if (type != intType) {
            return 0;
        }
        return static_cast<Buffer<int> *>(value);
    }
    Buffer<long> *getLongArray() {
        if (type != longType) {
            return 0;
        }
        return static_cast<Buffer<long> *>(value);
    }
    Buffer<char> *getCharArray() {
        if (type != charType) {
            return 0;
        }
        return static_cast<Buffer<char> *>(value);
    }
    Buffer<String> *getStringArray() {
        if (type != stringType) {
            return 0;
        }
        return static_cast<Buffer<String> *>(value);
    }
    Buffer<bool> *getBoolArray() {
        if (type != boolType) {
            return 0;
        }
        return static_cast<Buffer<bool> *>(value);
    }

    float getFloat(float defValue) {
        Buffer<float> *buf = getFloatArray();
        if (value == 0 || buf == 0 || buf->isEmpty()) {
            return defValue;
        }
        return buf->get(0);
    }
    double getDouble(double defValue) {
        Buffer<double> *buf = getDoubleArray();
        if (value == 0 || buf == 0 || buf->isEmpty()) {
            return defValue;
        }
        return buf->get(0);
    }
    int getInt(int defValue) {
        Buffer<int> *buf = getIntArray();
        if (value == 0 || buf == 0 || buf->isEmpty()) {
            return defValue;
        }
        return buf->get(0);
    }
    long getLong(long defValue) {
        Buffer<long> *buf = getLongArray();
        if (value == 0 || buf == 0 || buf->isEmpty()) {
            return defValue;
        }
        return buf->get(0);
    }
    char getChar(char defValue) {
        Buffer<char> *buf = getCharArray();
        if (value == 0 || buf == 0 || buf->isEmpty()) {
            return defValue;
        }
        return buf->get(0);
    }
    String getString(const String &defValue) {
        Buffer<String> *buf = getStringArray();
        if (value == 0 || buf == 0 || buf->isEmpty()) {
            return defValue;
        }
        return buf->get(0);
    }
    bool getBool(const bool defValue) {
        Buffer<bool> *buf = getBoolArray();
        if (value == 0 || buf == 0 || buf->isEmpty()) {
            return defValue;
        }
        return buf->get(0);
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
    void set(const String &name, const int type, bool isArray, void *value, String line) {

        Entry *entry = findEntry(name, false);
        if (entry == 0) {
            entry = new Entry(name);
            entry->line = line;
            buffer->append(entry);
        }

        entry->set(type, isArray, value);
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
        delete zeroEntry;
        buffer = 0;
        zeroEntry = 0;
    }

    void setLines(const Buffer<String> &lines) {
        for (int i = 0; i < lines.getLength(); i++) {
            setLine(lines.get(i));
        }
    }

    bool setLine(const String &line) {
        int idx = line.indexOf('=');
        if (!isValidLine(line, idx)) {
            return false;
        }

        String name = line.subStr(0, idx).trim();
        String value = line.subStr(idx + 1).trim();
        if (value.getLength() == 0) {
            // ignore this properties. use default value.
            return false;
        }
        String type;
        if (name.endWith(")")) {
            int idx2 = name.lastIndexOf('(');
            if (idx2 > 0) {
                type = name.subStr(idx2 + 1, name.getLength() - idx2 - 2);
                name = name.subStr(0, idx2);
            }
        }
        bool isArray = false;
        if (type.endWith("[]")) {
            isArray = true;
            type = type.subStr(0, type.getLength() - 2);
        }

        if (type == 0) {
            Buffer<String> vs;
            vs.append(value);
            this->set(name, isArray, vs, line);
        } else if (type == "float") {
            Buffer<float> vs = Float::parseAll<float>(value, ',', [](double d) { return (float)d; });
            this->set(name, isArray, vs, line);
        } else if (type == "int") {
            Buffer<int> vs = Float::parseAll<int>(value, ',', [](double d) { return (int)d; });
            this->set(name, isArray, vs, line);
        } else if (type == "long") {
            Buffer<long> vs = Float::parseAll<long>(value, ',', [](double d) { return (long)d; });
            this->set(name, isArray, vs, line);
        } else if (type == "double") {
            Buffer<double> vs = Float::parseAll<double>(value, ',', [](double d) { return (double)d; });
            this->set(name, isArray, vs, line);
        } else if (type == "string") {
            Buffer<String> vs = value.split(',');
            this->set(name, isArray, vs, line);
        } else if (type == "bool") {
            Buffer<bool> vs = value.split<bool>(',', [](String &value) {
                return (value == "true" || value == "y" || value == "TRUE" || value == "Y" || value == "ON");
            });

            this->set(name, isArray, vs, line);
        } else if (type == "char") {
            Buffer<char> vs = value.split<char>(',', [](String &value) {
                if (value == 0) {
                    return '\0';
                }
                return value.getChar(0);
            });

            this->set(name, isArray, vs, line);
        } else {
            // error process?
            this->set(name, value);
        }
        return true;
    }

    bool isValidLine(String line, int idxOfEq) {
        if (line.getLength() == 0) {
            //
            return false;
        }
        if (line.getChar(0) == '#') {
            return false;
        }

        if (idxOfEq < 0) {
            // ignore this line.
            return false;
        }
        if (idxOfEq == 0) {
            // ignore this line.
            return false;
        }
        return true;
    }

    void load(Reader &reader) {
        LineReader &&lr = LineReader(&reader);
        while (true) {
            String line;
            int ret = lr.readLine(line, false); //
            if (ret == 0) {
                break;
            }
            if (ret < 0) {
                // error
                break;
            }
            setLine(line);
        }
    }

    Buffer<String> getNames() {
        Buffer<String> ret;
        for (int i = 0; i < this->buffer->getLength(); i++) {
            ret.append(this->buffer->get(i)->name);
        }
        return ret;
    }
    // start set methods
    void set(const String &name, bool isArray, Buffer<int> value, String line) {
        Buffer<void *> bf2;
        this->set(name, intType, isArray, new Buffer<int>(value), line);
    }
    void set(const String &name, bool isArray, Buffer<char> value, String line) {
        this->set(name, charType, isArray, new Buffer<char>(value), line);
    }

    void set(const String &name, bool isArray, Buffer<long> value, String line) {
        this->set(name, longType, isArray, new Buffer<long>(value), line);
    }

    void set(const String &name, bool isArray, const Buffer<String> &value, String line) {
        this->set(name, stringType, isArray, new Buffer<String>(value), line);
    }

    void set(const String &name, bool isArray, const Buffer<bool> value, String line) {
        this->set(name, boolType, isArray, new Buffer<bool>(value), line);
    }

    void set(const String &name, bool isArray, const Buffer<float> value, String line) {
        this->set(name, floatType, isArray, new Buffer<float>(value), line);
    }

    void set(const String &name, bool isArray, const Buffer<double> value, String line) {
        this->set(name, doubleType, isArray, new Buffer<double>(value), line);
    }
    //
    void set(const String &name, int value) {
        Buffer<int> *buf = new Buffer<int>();
        buf->append(value);
        this->set(name, intType, false, buf, "");
    }

    void set(const String &name, char value) {
        Buffer<char> *buf = new Buffer<char>();
        buf->append(value);
        this->set(name, charType, false, buf, "");
    }

    void set(const String &name, long value) {
        Buffer<long> *buf = new Buffer<long>();
        buf->append(value);
        this->set(name, longType, false, buf, "");
    }
    void set(const String &name, const char *str) {
        set(name, String::string(str));
    }

    void set(const String &name, const String &value) {
        Buffer<String> *buf = new Buffer<String>();
        buf->append(value);
        this->set(name, stringType, false, buf, "");
    }

    void set(const String &name, const bool value) {
        Buffer<bool> *buf = new Buffer<bool>();
        buf->append(value);
        this->set(name, boolType, false, buf, "");
    }

    void set(const String &name, const float value) {
        Buffer<float> *buf = new Buffer<float>();
        buf->append(value);
        this->set(name, floatType, false, buf, "");
    }

    void set(const String &name, const double value) {
        Buffer<double> *buf = new Buffer<double>();
        buf->append(value);
        this->set(name, doubleType, false, buf, "");
    }

    // end set methods

    void *getExternal(const String &name, void *defValue) {
        return findEntry(name, true)->getExternal(defValue);
    }

    // start get methods

    float getFloat(const String &name, float defValue) {
        return findEntry(name, true)->getFloat(defValue);
    }

    double getDouble(const String &name, double defValue) {
        return findEntry(name, true)->getDouble(defValue);
    }

    int getInt(const String &name, int defValue) {
        return findEntry(name, true)->getInt(defValue);
    }

    long getLong(const String &name, long defValue) {
        return findEntry(name, true)->getLong(defValue);
    }
    char getChar(const String &name, long defValue) {
        return findEntry(name, true)->getChar(defValue);
    }

    String getString(const String &name, const String &defValue) {
        return findEntry(name, true)->getString(defValue);
    }
    bool getBool(const String &name, const bool defValue) {
        return findEntry(name, true)->getBool(defValue);
    }
    // get as array

    Buffer<float> *getFloatArray(const String &name) {
        return findEntry(name, true)->getFloatArray();
    }

    Buffer<double> *getDouble(const String &name) {
        return findEntry(name, true)->getDoubleArray();
    }

    Buffer<int> *getInt(const String &name) {
        return findEntry(name, true)->getIntArray();
    }

    Buffer<long> *getLong(const String &name) {
        return findEntry(name, true)->getLongArray();
    }
    Buffer<char> *getChar(const String &name) {
        return findEntry(name, true)->getCharArray();
    }

    Buffer<String> *getString(const String &name) {
        return findEntry(name, true)->getStringArray();
    }
    Buffer<bool> *getBool(const String &name) {
        return findEntry(name, true)->getBoolArray();
    }
    // end get methods
    void mergeFrom(Properties &pts) {
        for (int i = 0; i < pts.buffer->getLength(); i++) {
            Entry *entry = pts.buffer->get(i);
            this->set(entry->name, entry->type, entry->isArray, entry->value, entry->line);
        }
    }
    String getLine(String name) {
        Entry *entry = this->findEntry(name, false);
        if (entry == 0) {
            return "";
        }
        return entry->getLine();
    }
};

} // namespace  a8::util
