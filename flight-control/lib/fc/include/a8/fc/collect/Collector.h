#pragma once
#include "a8/fc/collect/DataItem.h"
#include "a8/util.h"
#define DEFAULT_TAIL_PRECISION (2)
#define DEFAULT_POINT_OFFSET (6)
namespace a8::fc::collect {
const Format::AutoOffsetFloat floatFormat(6, 3);
const Format::AutoOffsetFloat boolFormat(1, 0);
const Format::AutoOffsetFloat intFormat(8, 0);
const Format::AutoOffsetFloat longFormat(8, 0);

using namespace a8::util;
class Collector {

    class DataItemEntry {
    public:
        DataItem *dataItem;
        void (*release)(DataItem *);

        DataItemEntry(DataItem *di, void (*release)(DataItem *)) {
            this->dataItem = di;
            this->release = release;
        }
        ~DataItemEntry() {
            this->release(dataItem);
        }
    };

private:
    Collector(const Collector &);
    Collector &operator=(const Collector &);

    Buffer<DataItemEntry *> dataItems;
    HashTable<String, int> dataItemMap;
    Writer *writer;

public:
    Collector(Writer *writer) {
        this->writer = writer;
    }
    ~Collector() {
        this->dataItems.clear([](DataItemEntry *de) {
            delete de;
        });
    }

    DataItem *get(int idx) {
        DataItemEntry *de = dataItems.get(idx, 0);
        return de->dataItem;
    }

    DataItem *get(String name) {
        int idx = dataItemMap.get(name, -1);
        if (idx == -1) {
            return 0;
        }
        DataItemEntry *de = dataItems.get(idx, 0);
        return de->dataItem;
    }

    int add(DataItem *di, void (*release)(DataItem *)) {
        String name = di->getName();
        int idx = dataItemMap.get(name, -1);
        if (idx > -1) {
            return -1;
        }
        idx = dataItems.len();
        DataItemEntry *de = new DataItemEntry(di, release);
        this->dataItems.append(de);
        dataItemMap.set(name, idx);
        return idx;
    }

    template <typename C>
    int add(String name, C c, double (*getter)(C)) {
        return add(name, c, getter, &floatFormat);
    }
    template <typename C>
    int add(String name, C c, double (*getter)(C), const Format::Float *format) {
        DataItem *di = new FunctionalDataItem<C>(name, c, getter, format);
        int ret = this->add(di, [](DataItem *di) { delete di; });
        if (ret < 0) {
            delete di;
        }
        return ret;
    }

    template <typename T>
    int add(String name, T &var, const Format::Float *format) {
        DataItem *di = new BindDataItem<T>(name, var, format);
        int ret = this->add(di, [](DataItem *di) { delete di; });
        if (ret < 0) {
            delete di;
        }
        return ret;
    }

    template <typename T>
    int add(String name, T &var) {
        return add<T>(name, var, &floatFormat);
    }

    int add(String name, long &var) {
        return add<long>(name, var, &longFormat);
    }

    int add(String name, int &var) {
        return add<int>(name, var, &intFormat);
    }

    int add(String name, bool &var) {
        return add<bool>(name, var, &boolFormat);
    }

    int add(String name, float &var) {
        return add<float>(name, var, &floatFormat);
    }

    int add(String name, double &var) {
        return add<double>(name, var, &floatFormat);
    }

    void writeHeader() {
        for (int i = 0; i < dataItems.len(); i++) {
            DataItem *di = this->get(i);
            writer->write(di->getName());
            writer->write(",");
        }
        writer->write("\n");
    }
    void update() {
        String tmpStr;
        for (int i = 0; i < dataItems.len(); i++) {
            DataItem *di = this->get(i);
            tmpStr.setFloatFormat(di->format);
            double v = di->get();
            tmpStr << v;
            this->writer->write(tmpStr);
            this->writer->write(",");
            tmpStr.clear();
        }
        writer->write("\n");
    }
};
} // namespace a8::fc::collect