#pragma once
#include "a8/fc/collect/DataItems.h"
#include "a8/util.h"

namespace a8::fc::collect {
using namespace a8::util;

class NamedExpr {
public:
    static int parseAll(Buffer<String> namedExprStrs, Buffer<NamedExpr> *buf, Result &res) {
        int ret = 1;
        for (int i = 0; i < namedExprStrs.len(); i++) {
            NamedExpr nExpr;
            ret = parse(namedExprStrs.get(i, ""), nExpr, res);
            if (ret < 0) {
                break;
            }
            buf->append(nExpr);
        }
        return ret;
    }
    static int parse(String namedExprStr, NamedExpr &nExpr, Result &res) {
        String name;
        String func;
        Buffer<String> args;
        int idx = namedExprStr.indexOf('[');
        if (idx < 0) {
            name = namedExprStr;
        } else {
            if (!namedExprStr.endWith(']')) {
                res << "nameWithExpr must end with ']', namedExprStr?:" << namedExprStr;
                return -3;
            }
            name = namedExprStr.subStr(0, idx);
            String expr = namedExprStr.subStr(idx + 1, namedExprStr.len() - idx - 2);

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
        }
        nExpr = NamedExpr(name, func, args);
        return 1;
    }

public:
    String name;
    String func;
    Buffer<String> args;
    NamedExpr() {
    }
    NamedExpr(String name, String func, Buffer<String> args) {
        this->name = name;
        this->func = func;
        this->args = args;
    }

    int buildDataItem(DataItem *&di, Result &res) {

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
        return 1;
    }

    friend String &operator<<(String &str, const NamedExpr &expr) {
        str << "NamedExpr(" << expr.name << "," << expr.func << "(";
        for (int i = 0; i < expr.args.len(); i++) {
            str << expr.args.get(i, "") << ",";
        }
        str << "))";
        return str;
    }
};
} // namespace a8::fc::collect