#pragma once
#include "a8/fc/collect/Collect.h"
#include "a8/fc/collect/DataItem.h"
#include "a8/util.h"

namespace a8::fc::collect {
using namespace a8::util;

template <typename T>
class BindDataItem : public DataItem {

public:
    T &bind_;
    BindDataItem(String name, T &bind, const Format::Float *format) : DataItem(name, format), bind_(bind) {
    }
    int setup(Collect *collect, Result &res) override {
        return 1;
    }
    double get(Collect *collect) override {
        return double(this->bind_);
    }
};

template <typename C>
class FunctionalDataItem : public DataItem {
    using getter = double (*)(C);

public:
    getter getter_;
    C c2;
    FunctionalDataItem(String name, C c2, getter getter, const Format::Float *format) : DataItem(name, format), c2(c2) {
        this->getter_ = getter;
    }
    int setup(Collect *collect, Result &res) override {
        return 1;
    }
    double get(Collect *collect) override {
        return this->getter_(c2);
    }
};

class RowNumDataItem : public DataItem {
protected:
public:
    RowNumDataItem(String name, const Format::Float *format) : DataItem(name, format) {
    }
    int setup(Collect *collect, Result &res) override {
        return 1;
    }
    double get(Collect *collect) override {
        long rNum = collect->getRowNum();
        return rNum;
    }
};
class RefDataItem : public DataItem {
protected:
    Buffer<DataItem *> dis;
    Buffer<String> diNames;
    long previousRowNum = 0;
    double value; // cached value

public:
    RefDataItem(String name, String diName, const Format::Float *format) : DataItem(name, format) {
        this->diNames.add(diName);
    }

    RefDataItem(String name, Buffer<String> diNames, const Format::Float *format) : DataItem(name, format) {
        this->diNames.add(diNames);
    }

    int setup(Collect *collect, Result &res) override {
        for (int i = 0; i < diNames.len(); i++) {
            String diName = this->diNames.get(i, "");
            DataItem *di = collect->get(diName);
            if (di == 0) {
                res << "no such di with name:" << diName;
                return -1;
            }
            this->dis.add(di);
        }
        if (this->dis.isEmpty()) {
            return -1;
        }

        return 1;
    }
    double get(Collect *collect) override {
        long rNum = collect->getRowNum();
        if (this->previousRowNum == rNum) {
            return value;
        }
        if (this->previousRowNum != rNum - 1) {
            return 0; // unknown value.
        }
        int len = dis.len();
        double diValues[len];
        for (int i = 0; i < len; i++) {
            diValues[i] = dis.get(i, 0)->get(collect);
        }

        this->update(this->value, rNum, diValues, dis.len());
        this->previousRowNum = rNum;
        return this->value;
    }

    virtual void update(double &value, long rNum, double *diValues, int len) = 0;
};

class MultipleDataItem : public RefDataItem {
    double factor;

public:
    MultipleDataItem(String name, String diName, double factor, const Format::Float *format) : RefDataItem(name, diName, format) {
        this->factor = factor;
    }

    void update(double &value, long rNum, double *diValues, int len) override {
        value = diValues[0] * this->factor;
    }
};

class DiffDataItem : public RefDataItem {
    double value = 0; // cached value.
    long rowNum = -1;
    double diPreviousValue = 0;

public:
    DiffDataItem(String name, String diName, const Format::Float *format) : RefDataItem(name, diName, format) {
    }
    void update(double &value, long rNum, double *diValues, int len) override {
        if (rNum == 1) {
            value = 0;
        } else {
            value = diValues[0] - diPreviousValue;
        }
        this->diPreviousValue = diValues[0];
    }
};

class AvgDataItem : public RefDataItem {
    double total = 0;

public:
    AvgDataItem(String name, String diName, const Format::Float *format) : RefDataItem(name, diName, format) {
    }
    void update(double &value, long rNum, double *diValues, int len) override {
        this->total += diValues[0];
        value = this->total / rNum;
    }
};

class MaxDataItem : public RefDataItem {
public:
    MaxDataItem(String name, String diName, const Format::Float *format) : RefDataItem(name, diName, format) {
    }

    void update(double &value, long rNum, double *diValues, int len) override {

        if (rNum == 1 || value < diValues[0]) {
            value = diValues[0];
        }
        //
    }
};

class MinDataItem : public RefDataItem {
    long rowNum = 0;
    double value; // cached value.

public:
    MinDataItem(String name, String diName, const Format::Float *format) : RefDataItem(name, diName, format) {
    }
    void update(double &value, long rNum, double *diValues, int len) override {

        if (rNum == 1 || value > diValues[0]) {
            value = diValues[0];
        }
        //
    }
};

class MaxOfDataItem : public RefDataItem {
public:
    MaxOfDataItem(String name, Buffer<String> diNames, const Format::Float *format) : RefDataItem(name, diNames, format) {
    }

    void update(double &value, long rNum, double *diValues, int len) override {
        double d = diValues[0];
        for (int i = 1; i < len; i++) {
            if (d < diValues[i]) {
                d = diValues[i];
            }
        }
        value = d;
        //
    }
};
} // namespace a8::fc::collect