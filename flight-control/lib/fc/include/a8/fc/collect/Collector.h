#pragma once
#include "a8/fc/collect/DataItem.h"
#include "a8/util.h"
#define DEFAULT_TAIL_PRECISION (2)
#define DEFAULT_POINT_OFFSET (6)
namespace a8::fc::collect {


const Format::AutoOffsetFloat doubleFormat(15, 3);
const Format::AutoOffsetFloat noTailDoubleFormat(15, 0);

using namespace a8::util;
class Collector {

    class DataItemEntry {
    public:
        DataItem *dataItem;
        void (*release)(DataItem *);
        bool enable = true;
        DataItemEntry(DataItem *di, void (*release)(DataItem *)) {
            this->dataItem = di;
            this->release = release;
        }
        ~DataItemEntry() {
            this->release(dataItem);
        }
        void setEnable(bool enable) {
            this->enable = enable;
        }
    };

private:
    Collector(const Collector &);
    Collector &operator=(const Collector &);

    Buffer<DataItemEntry *> dataItemEntries;
    HashTable<String, int> dataItemMap;
    HashTable<String, bool> dataItemEnableStatus;
    Buffer<DataItem *> enabledDataItems;

    bool defaultEnable = true;
    Writer *writer;

public:
    Collector(Writer *writer) {
        this->writer = writer;
    }
    ~Collector() {
        this->dataItemEntries.clear([](DataItemEntry *de) {
            delete de;
        });
    }

    DataItem *get(int idx) {
        DataItemEntry *de = dataItemEntries.get(idx, 0);
        return de->dataItem;
    }

    DataItem *get(String name) {
        int idx = dataItemMap.get(name, -1);
        if (idx == -1) {
            return 0;
        }
        DataItemEntry *de = dataItemEntries.get(idx, 0);
        return de->dataItem;
    }

    void enable(Buffer<String> names, bool enable) {
        for (int i = 0; i < names.len(); i++) {
            String name = names.get(i, "");
            dataItemEnableStatus.set(name, enable);
        }
    }

    void setDefaultEnable(bool enable) {
        this->defaultEnable = enable;
    }

    int add(DataItem *di, void (*release)(DataItem *)) {
        String name = di->getName();
        int idx = dataItemMap.get(name, -1);
        if (idx > -1) {
            return -1;
        }
        idx = dataItemEntries.len();
        DataItemEntry *de = new DataItemEntry(di, release);
        this->dataItemEntries.append(de);
        dataItemMap.set(name, idx);
        return idx;
    }

    template <typename C>
    int add(String name, C c, double (*getter)(C)) {
        return add(name, c, getter, &doubleFormat);
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
        return add<T>(name, var, &doubleFormat);
    }

    int add(String name, long &var) {
        return add<long>(name, var, &noTailDoubleFormat);
    }

    int add(String name, int &var) {
        return add<int>(name, var, &noTailDoubleFormat);
    }

    int add(String name, bool &var) {
        return add<bool>(name, var, &noTailDoubleFormat);
    }

    int add(String name, float &var) {
        return add<float>(name, var, &doubleFormat);
    }

    int add(String name, double &var) {
        return add<double>(name, var, &doubleFormat);
    }

    void preWrite() {
        for (int i = 0; i < this->dataItemEntries.len(); i++) {
            DataItemEntry *de = dataItemEntries.get(i, 0);
            String name = de->dataItem->getName();
            bool enable = dataItemEnableStatus.get(name, this->defaultEnable);
            if (!enable) {
                continue;
            }
            enabledDataItems.add(de->dataItem);
        }
    }
    void writeHeader() {
        for (int i = 0; i < enabledDataItems.len(); i++) {            
            DataItem *di = this->enabledDataItems.get(i, 0);
            writer->write(di->getName());
            writer->write(",");
        }
        writer->write("\n");
    }
    void update() {
        String tmpStr;
        for (int i = 0; i < enabledDataItems.len(); i++) {
            DataItem *di = this->enabledDataItems.get(i, 0);
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