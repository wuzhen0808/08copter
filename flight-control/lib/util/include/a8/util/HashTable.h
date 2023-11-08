#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/LinkedList.h"

namespace a8::util {

template <typename K, typename V>
class HashTable {

    template <typename K1, typename V1>
    struct Entry {
        K1 k;
        V1 v;
    };

    using hashCode = int (*)(K);
    using equals = bool (*)(K, K);

    Buffer<LinkedList<Entry<K, V>> *> table;
    hashCode hashCode_;
    equals equals_;

    int remove(K key, V *&removedBuffer) {
        bool appendBuffer = removedBuffer != 0;

        int code = hashCode_(key);

        LinkedList<Entry<K, V>> *list = table.get(code, 0);
        if (list == 0) {
            return false;
        }

        struct Context {
            equals equals_;
            K key;
        };

        Context context;
        context.equals_ = this->equals_;
        context.key = key;
        Entry<K, V> *entryBuffer = appendBuffer ? new Entry<K, V>[0] : 0;

        int len = list->template remove<Context *>(
            &context, [](Context *context, Entry<K, V> e1) {
                return context->equals_(e1.k, context->key);
            },
            entryBuffer);

        if (entryBuffer != 0) {
            int len2 = 0;
            int cap2 = 0;
            for (int i = 0; i < len; i++) {
                Entry<K, V> entry = entryBuffer[i];
                Lang::append<V>(removedBuffer, len2, cap2, 16, 0, &entry.v, 0, 1);
            }
            delete[] entryBuffer;
        }
        return len;
    }

public:
    HashTable(hashCode hashCodeF, equals equalsF) {
        this->hashCode_ = hashCodeF;
        this->equals_ = equalsF;
    }

    ~HashTable() {
        this->clear();
    }

    void set(K key, V value) {
        int code = hashCode_(key);

        LinkedList<Entry<K, V>> *list = table.get(code, 0);
        if (list == 0) {
            list = new LinkedList<Entry<K, V>>();
            table.set(code, 0, list);
        }
        Entry<K, V> entry;
        entry.k = key;
        entry.v = value;

        struct Params {
            K key_;
            V value_;
            equals equals_;
        };

        Params p;
        p.key_ = key;
        p.value_ = value;
        p.equals_ = this->equals_;

        list->template replaceOrAdd<Params *>(&p, entry, [](Params *p, Entry<K, V> entry2) {
            return p->equals_(p->key_, entry2.k);
        });
    }

    V get(K key, V def) {
        struct Params {
            K key_;
            V value_;
            int counter_ = 0;
        };
        Params p;
        p.key_ = key;
        int len = get<Params *>(key, &p, [](Params *p, V value) {
            if (p->counter_ == 0) {
                p->value_ = value;
            }
            p->counter_++;
        });

        if (len == 0) {
            return def;
        }
        return p.value_;
    }

    template <typename C>
    int get(K key, C context, void (*consume)(C, V)) {
        int code = hashCode_(key);
        LinkedList<Entry<K, V>> *list = table.get(code, 0);
        if (list == 0) {
            return false;
        }
        struct Params {
            K key_;
            C context_;
            void (*consume_)(C, V);
            equals equals_;
        };
        Params p;
        p.context_ = context;
        p.consume_ = consume;
        p.key_ = key;
        p.equals_ = this->equals_;

        return list->template find<Params *>(
            &p,                                                                        //
            [](Params *p, Entry<K, V> entry) { return p->equals_(p->key_, entry.k); }, //
            [](Params *p, Entry<K, V> entry) {
                p->consume_(p->context_, entry.v);
            } //
        );
    }

    void clear() {
        for (int i = 0; i < table.len(); i++) {
            LinkedList<Entry<K, V>> *list = table.get(i);
            if (list == 0) {
                continue;
            }
            delete list;
        }
    }
    int remove(K key) {
        V *buffer = 0;
        int len = this->remove(key, buffer);
        return len;
    }

    int remove(K key, void (*afterRemove)(V)) {
        V *buffer = new V[0];
        int len = this->remove(key, buffer);
        for (int i = 0; i < len; i++) {
            afterRemove(buffer[i]);
        }
        delete[] buffer;
        return len;
    }
};
} // namespace  a8::util
