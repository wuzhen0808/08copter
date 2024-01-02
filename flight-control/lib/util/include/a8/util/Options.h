/*
 *
 * Copyright (c) 2024 Wu Zhen(wuzhen0808@163.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next paragraph)
 * shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#pragma once
#include "a8/util/String.h"
#include "a8/util/HashTable.h"
#include "a8/util/Buffer.h"
namespace a8::util {
template <typename T>
class Options : public HashTable<T, String> {
public:    
    Options() {
    }
    void add(T key, String name) {
        HashTable<T, String>::set(key, name);
    }
    String getName(T value) {
        return HashTable<T, String>::get(value);
    }
    void getValueAndNames(Buffer<T> &values, Buffer<String> &names) {
        struct Context {
            Buffer<T> &values;
            Buffer<String> &names;
            Context(Buffer<T> &values, Buffer<String> &names) : values(values), names(names) {
            }
        } p(values, names);
        HashTable<T, String>::template forEach<Context *>(&p, [](Context *p, T k, String v) {
            p->values.add(k);
            p->names.add(v);
        });
    }
};
} // namespace a8::util