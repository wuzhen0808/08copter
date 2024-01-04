#pragma once
#include "a8/fc/collect/Collect.h"
#include "a8/fc/collect/DataItems.h"
#include "a8/fc/collect/NamedExpr.h"
#include "a8/util.h"
#include "a8/util/sched.h"
#define DEFAULT_TAIL_PRECISION (2)
#define DEFAULT_POINT_OFFSET (6)
namespace a8::fc::collect {

using namespace a8::util;
using namespace a8::util::sched;

class Collector : public Collect {

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
    HashTable<String, bool> dataItemActiveStatus;
    HashTable<String, int> sort;
    Buffer<DataItem *> enabledDataItems;

    bool defaultEnable = true;
    Writer *writer;

    SyncQueue<double *> *queue;

    bool running = true;

    long lost = 0;

    int rowWidth = -1;

public:
    Collector(SyncQueue<double *> *queue, Writer *writer) {
        this->queue = queue;
        this->writer = writer;
    }
    ~Collector() {
        this->dataItemEntries.clear([](DataItemEntry *de) {
            delete de;
        });
    }

    void close() {
        this->running = false;
    }

    void printActiveDataItems(Output *out) {
        printDataItems(out, false, true);
    }

    void printAllDataItems(Output *out) {
        printDataItems(out, true, false);
    }

    void printDataItems(Output *out, bool all, bool onlyActive) {
        int counter = 0;
        for (int i = 0; i < dataItemEntries.len(); i++) {
            DataItem *di = BufferUtil::get<DataItemEntry>(dataItemEntries, i)->dataItem;
            bool enabled = isEnabled(di->getName());
            if (all || onlyActive && enabled) {
                doPrint(counter++, i, di, enabled, out);
            }
        }
    }
    void doPrint(int idx, int idx2, DataItem *di, bool enabled, Output *out) {
        out->print(idx);
        out->print(",");
        out->print(idx2);
        out->print(",");
        out->print(di->getName());
        out->print(",");
        out->print<bool>(enabled);
        out->println();
    }

    bool isEnabled(String name) {
        return dataItemActiveStatus.get(name, false);
    }
    DataItem *get(int idx) {
        DataItemEntry *de = dataItemEntries.get(idx, 0);
        return de->dataItem;
    }

    DataItem *get(String name) override {
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
            dataItemActiveStatus.set(name, enable);
            sort.set(name, i);
        }
    }

    void setDefaultEnable(bool enable) {
        this->defaultEnable = enable;
    }

    int add(DataItem *di, void (*release)(DataItem *), Result &res) {
        String name = di->getName();
        int idx = dataItemMap.get(name, -1);
        if (idx > -1) {
            res << "duplicated dataItem:" << name;
            return -1;
        }
        idx = dataItemEntries.len();
        DataItemEntry *de = new DataItemEntry(di, release);
        this->dataItemEntries.append(de);
        dataItemMap.set(name, idx);
        return idx;
    }

    template <typename C>
    int add(String name, C c, double (*getter)(C), Result &res) {
        return add(name, c, getter, &doubleFormat, res);
    }
    template <typename C>
    int add(String name, C c, double (*getter)(C), const Format::Float *format, Result &res) {
        DataItem *di = new FunctionalDataItem<C>(name, c, getter, format);
        int ret = this->add(
            di, [](DataItem *di) { delete di; }, res);
        if (ret < 0) {
            delete di;
        }
        return ret;
    }

    template <typename T>
    int add(String name, T &var, const Format::Float *format, Result &res) {
        DataItem *di = new BindDataItem<T>(name, var, format);
        int ret = this->add(
            di, [](DataItem *di) { delete di; }, res);
        if (ret < 0) {
            delete di;
        }
        return ret;
    }

    int add(String name, long &var, Result &res) {
        return add<long>(name, var, &longFormat, res);
    }

    int add(String name, unsigned long &var, Result &res) {
        return add<unsigned long>(name, var, &longFormat, res);
    }

    int add(String name, int &var, Result &res) {
        return add<int>(name, var, &intFormat, res);
    }

    int add(String name, bool &var, Result &res) {
        return add<bool>(name, var, &boolFormat, res);
    }

    int add(String name, float &var, Result &res) {
        return add<float>(name, var, &floatFormat, res);
    }

    int add(String name, double &var, Result &res) {
        return add<double>(name, var, &doubleFormat, res);
    }

    int add(NamedExpr namedExpr, DataItem *&di, Result &res) {
        int ret = namedExpr.buildDataItem(di, res);
        if (ret < 0) {
            return ret;
        }
        ret = this->add(
            di, [](DataItem *di) { delete di; }, res);
        return ret;
    }
    bool contains(String name) {
        DataItem *di = this->get(name);
        return di != 0;
    }

    int addAllIfNotExists(Buffer<NamedExpr> nExprs, Result &res) {
        int ret = 1;
        for (int i = 0; i < nExprs.len(); i++) {
            NamedExpr nExpr = nExprs.get(i, NamedExpr());
            if (this->contains(nExpr.name)) {
                continue; // ignore if exists.
            }
            DataItem *di;
            ret = nExpr.buildDataItem(di, res);
            if (ret < 0) {
                res << ";no such di(" << nExpr.name << ") or cannot parse as expr.";
                break;
            }
            ret = this->add(di, Lang::delete_<DataItem>, res);
            if (ret < 0) {
                res << (String() << ";no such dataItem or not a expr:" << nExpr);
                break;
            }
        }
        return ret;
    }
    /**
     * Do not add or modify data item after setup.
     */
    int setup(Result &res) {

        if (rowWidth >= 0) {
            res << "cannot setup twice.";
            return -1;
        }

        int ret = -1;
        for (int i = 0; i < this->dataItemEntries.len(); i++) {
            DataItem *di = this->get(i);
            ret = di->setup(this, res);
            if (ret < 0) {
                break;
            }
        }
        if (ret < 0) {
            return ret;
        }

        for (int i = 0; i < this->dataItemEntries.len(); i++) {
            DataItemEntry *de = dataItemEntries.get(i, 0);
            String name = de->dataItem->getName();
            bool enable = dataItemActiveStatus.get(name, this->defaultEnable);
            if (!enable) {
                continue;
            }
            enabledDataItems.add(de->dataItem);
        }
        enabledDataItems.sort<Collector *>(this, [](Collector *this_, DataItem *di1, DataItem *di2) {
            int s1 = this_->sort.get(di1->name, 0);
            int s2 = this_->sort.get(di2->name, 0);
            return s1 - s2;
        });

        this->rowNum = 0;
        this->rowWidth = enabledDataItems.len();
        this->queue->clear();
        return ret;
    }

    void update() {
        this->rowNum++;
        double *row = new double[rowWidth]; // delete at run();
        for (int i = 0; i < rowWidth; i++) {
            DataItem *di = this->enabledDataItems.get(i, 0);
            row[i] = di->get(this);
        }

        int ok = this->queue->offer(row, 0);
        if (ok < 0) {
            lost++;
        }
    }

    int run(Result &res) {
        if (rowWidth < 0) {
            res << "please setup() before run().";
            return -1;
        }

        String cells[rowWidth];

        int counter = 0;
        while (this->running) {
            double *row;
            int got = queue->take(row, 200);
            if (got < 0) {
                continue;
            }
            if (row == 0) {
                // null row indicate the end.
                this->running = false;
                break;
            }
            if (counter == 0) {
                for (int i = 0; i < rowWidth; i++) {
                    DataItem *di = this->enabledDataItems.get(i, 0);
                    cells[i].setFloatFormat(di->format);
                    writer->write(di->getName());
                    writer->write(",");
                }
                writer->write("\n");
            }
            counter++;
            for (int i = 0; i < rowWidth; i++) {
                String &cell = cells[i];
                cell << row[i];
                this->writer->write(cell);
                this->writer->write(",");
                cell.clear();
            }
            writer->write("\n");
            delete[] row; // delete here, create at update();
        }
        return 1;
    }
};
} // namespace a8::fc::collect