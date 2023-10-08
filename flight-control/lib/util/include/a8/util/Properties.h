#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Float.h"
#include "a8/util/LineReader.h"
#include "a8/util/Reader.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"

#define PT_FLOAT ("float")
#define PT_INT ("int")
#define PT_STRING ("string")

namespace a8::util {

class Properties {

    class Entry {
    public:
        String name;
        String type;
        String value;

        Entry(const String name, const String type, const String value) {
            set(name, type, value);
        }

        void set(const String name, const String type, const String value) {
            this->name = name;
            this->type = type;
            this->value = value;
        }
        ~Entry() {
        }
        String getLine() {

            return name + "=" + value;
        }

        Buffer<float> getFloatArray() {
            Buffer<float> ret;
            if (type != PT_FLOAT) {
                return ret;
            }
            ret = Float::parseAll<float>(value, ',', [](double d) { return (float)d; });

            return ret;
        }

        Buffer<int> getIntArray() {
            Buffer<int> ret;
            if (type != PT_INT) {
                return ret;
            }
            ret = Float::parseAll<int>(value, ',', [](double d) { return (int)d; });
            return ret;
        }

        Buffer<String> getStringArray() {
            Buffer<String> ret;
            if (type != PT_STRING) {
                return ret;
            }
            ret = StringUtil::split(value, ',');
            return ret;
        }

        float getFloat(float defValue) {
            Buffer<float> buf = getFloatArray();
            if (value == 0 || buf.isEmpty()) {
                return defValue;
            }
            return buf.get(0);
        }
        int getInt(int defValue) {
            Buffer<int> buf = getIntArray();
            if (value == 0 || buf.isEmpty()) {
                return defValue;
            }
            return buf.get(0);
        }
        String getString(String defValue) {
            Buffer<String> buf = getStringArray();
            if (value == 0 || buf.isEmpty()) {
                return defValue;
            }
            return buf.get(0);
        }
    };
    Entry *zeroEntry;

    Buffer<Entry *> *buffer;

    // TODO use a index tree or hash table.
    void set(const String name, const String type, String value) {

        Entry *entry = findEntry(name, false);
        if (entry == 0) {
            entry = new Entry(name, type, value);
            buffer->append(entry);
        }

        entry->set(name, type, value);
    }

    Entry *findEntry(const String &name, bool useZeroEntry) {
        for (int i = 0; i < buffer->length(); i++) {
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
        this->zeroEntry = new Entry("0", "0", "0");
        this->buffer = new Buffer<Entry *>();
    }

    ~Properties() {

        for (int j = 0; j < buffer->length(); j++) {
            Entry *entry = buffer->get(j);
            delete entry;
        }
        delete buffer;
        delete zeroEntry;
    }

    void setLines(const Buffer<String> &lines) {
        for (int i = 0; i < lines.length(); i++) {
            String str = lines.get(i);
            if (str.length() > 10000) {
                Lang::bug();
            }
            setLine(str);
        }
    }

    bool setLine(const String line) {
        int idx = line.indexOf('=');
        if (!isValidLine(line, idx)) {
            return false;
        }

        String name = line.subStr(0, idx).trim();
        String value = line.subStr(idx + 1).trim();
        if (value.length() == 0) {
            // ignore this properties-> use default value.
            return false;
        }
        String type;
        if (name.endWith(")")) {
            int idx2 = name.lastIndexOf('(');
            if (idx2 > 0) {
                type = name.subStr(idx2 + 1, name.length() - idx2 - 2);
                name = name.subStr(0, idx2);
            }
        } else {
            type = PT_STRING;
        }
        bool isArray = false;
        if (type.endWith("[]")) {
            isArray = true;
            type = type.subStr(0, type.length() - 2);
        }
        set(name, type, value);
        return true;
    }

    bool isValidLine(String line, int idxOfEq) {
        if (line.length() == 0) {
            //
            return false;
        }
        if (line.charAt(0) == '#') {
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
    void write(Writer &writer) {
        for (int i = 0; i < buffer->length(); i++) {
            Entry *entry = buffer->get(i);
            writer.write(entry->name.text(), entry->name.length());
            writer.write("=", 1);
            writer.write(entry->value.text(), entry->value.length());
            writer.write("\n", 1);
        }
    }

    Buffer<String> getNames() {
        Buffer<String> ret;
        for (int i = 0; i < this->buffer->length(); i++) {
            ret.append(this->buffer->get(i)->name);
        }
        return ret;
    }

    //
    void set(const String name, float value) {
        set(name, "float", String::format("%e", value));
    }

    void set(const String name, int value) {
        set(name, "int", String::format("%i", value));
    }

    void set(const String &name, const char *str) {
        set(name, String::string(str));
    }

    void set(const String &name, const String &value) {
        set(name, "string", value);
    }
    // start get methods

    float getFloat(const String &name, float defValue) {
        return findEntry(name, true)->getFloat(defValue);
    }

    int getInt(const String &name, int defValue) {
        return findEntry(name, true)->getInt(defValue);
    }

    String getString(const String &name, const String &defValue) {
        return findEntry(name, true)->getString(defValue);
    }

    // get as array

    Buffer<float> getFloatArray(const String &name) {
        return findEntry(name, true)->getFloatArray();
    }

    Buffer<int> getIntArray(const String &name) {
        return findEntry(name, true)->getIntArray();
    }

    Buffer<String> getStringArray(const String &name) {
        return findEntry(name, true)->getStringArray();
    }
    // end get methods
    void mergeFrom(Properties *pts, bool overwrite) {

        for (int i = 0; i < pts->buffer->length(); i++) {
            Entry *entry = pts->buffer->get(i);
            if (overwrite && this->contains(entry->name)) {
                continue;
            }

            this->set(entry->name, entry->type, entry->value);
        }
    }

    bool contains(String name) {
        Entry *entry = findEntry(name, false);
        return entry != 0;
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
