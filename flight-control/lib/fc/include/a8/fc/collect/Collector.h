#pragma once
#include "a8/fc/collect/Context.h"
#include "a8/fc/collect/DataItem.h"
#include "a8/util.h"

namespace a8::fc::collect {
using namespace a8::util;
class Collector : public FlyWeight {
    Buffer<DataItem *> dataItems;

public:
    Collector(LoggerFactory *logFac) : FlyWeight(logFac, "Collector") {
    }
    void add(DataItem *di) {
        this->dataItems.append(di);
    }

    template <typename C>
    void add(String name, C c, double (*getter)(C c)) {
        this->add(new FunctionalDataItem<C>(name, c, getter));
    }

    void writeHeader(Context &c) {
        for (int i = 0; i < dataItems.len(); i++) {
            DataItem *di = this->dataItems.get(i, 0);
            c.writer->write(di->getName());
            c.writer->write(",");
        }
        c.writer->write("\n");
    }
    void update(Context &c) {

        for (int i = 0; i < dataItems.len(); i++) {
            DataItem *di = dataItems.get(i, 0);
            double v = di->get();
            c.writer->write(String() << v);
            c.writer->write(",");
        }
        c.writer->write("\n");
    }
};
} // namespace a8::fc::collect