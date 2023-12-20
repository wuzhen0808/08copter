#pragma once
#include "a8/fc/collect/DataItem.h"
#include "a8/util.h"

namespace a8::fc::collect {
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
        DataItem *di = new FunctionalDataItem<C>(name, c, getter);

        int ret = this->add(di, [](DataItem *di) { delete di; });
        if (ret < 0) {
            delete di;
        }
        return ret;
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

        for (int i = 0; i < dataItems.len(); i++) {
            DataItem *di = this->get(i);
            double v = di->get();
            this->writer->write(String() << v);
            this->writer->write(",");
        }
        writer->write("\n");
    }
};
} // namespace a8::fc::collect