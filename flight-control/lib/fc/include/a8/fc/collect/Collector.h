#pragma once
#include "a8/fc/collect/Collect.h"
#include "a8/fc/collect/DataItems.h"
#include "a8/util.h"
#define DEFAULT_TAIL_PRECISION (2)
#define DEFAULT_POINT_OFFSET (6)
namespace a8::fc::collect {

using namespace a8::util;
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
    HashTable<String, bool> dataItemEnableStatus;
    Buffer<DataItem *> enabledDataItems;

    bool defaultEnable = true;
    Writer *writer;
    bool writeName = false;

public:
    Collector(Writer *writer) {
        this->writer = writer;
    }
    ~Collector() {
        this->dataItemEntries.clear([](DataItemEntry *de) {
            delete de;
        });
    }

    void printActiveDataItems(Output *out) {
        for (int i = 0; i < enabledDataItems.len(); i++) {
            DataItem *di = BufferUtil::get<DataItem>(enabledDataItems, i);
            doPrint(i, di, out);
        }
    }
    
    void printAllDataItems(Output *out) {
        for (int i = 0; i < dataItemEntries.len(); i++) {
            DataItem *di = BufferUtil::get<DataItemEntry>(dataItemEntries, i)->dataItem;
            doPrint(i, di, out);
        }
    }
    void doPrint(int idx, DataItem *di, Output *out) {
        out->print(idx);
        out->print(",");
        out->print(di->getName());
        out->print(",");
        out->print<bool>(isEnabled(di->getName()));
        out->println();
    }

    bool isEnabled(String name) {
        return dataItemEnableStatus.get(name, false);
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
            dataItemEnableStatus.set(name, enable);
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

    template <typename T>
    int add(String name, T &var, Result &res) {
        return add<T>(name, var, &doubleFormat, res);
    }

    int add(String name, long &var, Result &res) {
        return add<long>(name, var, &noTailDoubleFormat, res);
    }

    int add(String name, int &var, Result &res) {
        return add<int>(name, var, &noTailDoubleFormat, res);
    }

    int add(String name, bool &var, Result &res) {
        return add<bool>(name, var, &noTailDoubleFormat, res);
    }

    int add(String name, float &var, Result &res) {
        return add<float>(name, var, &doubleFormat, res);
    }

    int add(String name, double &var, Result &res) {
        return add<double>(name, var, &doubleFormat, res);
    }

    int add(String name, String diName, double factor, const Format::Float *format, Result &res) {
        return add(
            new MultipleDataItem(name, diName, factor, format), [](DataItem *di) { delete di; }, res);
    }

    int add(String name, String diName, double factor, Result &res) {
        return add(name, diName, factor, &doubleFormat, res);
    }

    int addNameWithExpr(String nameWithExpr, DataItem *&di, Result &res) {

        int idx = nameWithExpr.indexOf('[');
        if (idx < 0) {
            res << "not a nameWithExpr:" << nameWithExpr;
            return -2;
        }
        if (!nameWithExpr.endWith(']')) {
            res << "nameWithExpr must end with ']', nameWithExpr?:" << nameWithExpr;
            return -3;
        }
        String name = nameWithExpr.subStr(0, idx);
        String expr = nameWithExpr.subStr(idx + 1, nameWithExpr.len() - idx - 2);
        String func;
        Buffer<String> args;
        idx = expr.indexOf('(');
        if (idx > 0) {
            if (!expr.endWith(')')) {
                res << "expr must end with ')',expr:" << expr;
                return -4;
            }
            func = expr.subStr(0, idx);
            String argListS = expr.subStr(idx + 1, expr.len() - idx - 2);
            args = StringUtil::split(argListS, ',');
        } else {
            func = expr;
        }

        if (func == "rowNum") {
            di = new RowNumDataItem(name, &noTailDoubleFormat);
        } else if (func == "diff") {
            if (args.isEmpty()) {
                res << "diff arg was wrong.";
                return -5;
            }
            di = new DiffDataItem(name, args.get(0, ""), &doubleFormat);
        } else if (func == "*1000") {
            if (args.isEmpty()) {
                res << "multiple arg was wrong.";
                return -6;
            }
            di = new MultipleDataItem(name, args.get(0, ""), 1000, &doubleFormat);
        } else if (func == "avg") {
            if (args.isEmpty()) {
                res << "avg arg was wrong.";
                return -7;
            }
            di = new AvgDataItem(name, args.get(0, ""), &doubleFormat);
        } else if (func == "max") {
            if (args.isEmpty()) {
                res << "max arg was wrong.";
                return -8;
            }
            di = new MaxDataItem(name, args.get(0, ""), &doubleFormat);
        } else if (func == "min") {
            if (args.isEmpty()) {
                res << "min arg was wrong.";
                return -9;
            }
            di = new MinDataItem(name, args.get(0, ""), &doubleFormat);
        } else if (func == "maxOf") {
            di = new MaxOfDataItem(name, args, &doubleFormat);
        } else {
            res << "no such func:" << func;
            return -10;
        }

        int ret = this->add(
            di, [](DataItem *di) { delete di; }, res);
        return ret;
    }
    bool contains(String name) {
        DataItem *di = this->get(name);
        return di != 0;
    }

    int addAllIfNotExists(Buffer<String> nameWithExprs, Buffer<String> &names, Result &res) {
        int ret = 1;
        for (int i = 0; i < nameWithExprs.len(); i++) {
            String nameWithExpr = nameWithExprs.get(i, "");
            if (this->contains(nameWithExpr)) {
                continue; // ignore if exists.
            }
            DataItem *di;
            ret = this->addNameWithExpr(nameWithExpr, di, res);
            if (ret < 0) {
                res << ";no such dataItem or not a expr:" << nameWithExpr << "";
                break;
            }
            names.add(di->getName());
        }
        return ret;
    }
    int setup(Result &res) {
        int ret = -1;
        for (int i = 0; i < this->dataItemEntries.len(); i++) {
            DataItem *di = this->get(i);
            ret = di->setup(this, res);
            if (ret < 0) {
                break;
            }
        }

        return ret;
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
        this->rowNum = 0;
    }
    void writeHeader() {
        for (int i = 0; i < enabledDataItems.len(); i++) {
            DataItem *di = this->enabledDataItems.get(i, 0);
            if (this->writeName) {
                writer->write(di->getName());
                writer->write("-Name,");
            }
            writer->write(di->getName());
            writer->write(",");
        }
        writer->write("\n");
    }
    void update() {
        this->rowNum++;
        String tmpStr;
        for (int i = 0; i < enabledDataItems.len(); i++) {
            DataItem *di = this->enabledDataItems.get(i, 0);
            if (this->writeName) {
                tmpStr << di->getName();
                tmpStr << ",";
            }
            tmpStr.setFloatFormat(di->format);
            double v = di->get(this);
            tmpStr << v;
            this->writer->write(tmpStr);
            this->writer->write(",");
            tmpStr.clear();
        }
        writer->write("\n");
    }
};
} // namespace a8::fc::collect