#pragma once
#include "a8/fc/collect/Context.h"
#include "a8/fc/collect/DataItem.h"

namespace a8::fc::collector {
class Collector : public FlyWeight {
    Buffer<DataItem *> dataItems;
    Writer *writer;

public:
    Collector(Writer *writer, LoggerFactory *logFac) : FlyWeight(logFac, "Collector") {
        this->writer = writer;
    }
    void add(DataItem *di) {
        this->dataItems.append(di);
    }

    template <typename C>
    void add(String name, C c, float (*getter)(C c)) {
        this->add(new FunctionalDataItem(name, c, getter));
    }

    void writeHeader(){
        for(int i=0;i<dataItems.len();i++){
            DataItem * di = this->dataItems.get(i, 0);
            //writer->write();
        }
    }
    void update(Context &c) {
        for(int i=0;i<dataItems.len();i++){

        }
    }
};
} // namespace a8::fc::collector