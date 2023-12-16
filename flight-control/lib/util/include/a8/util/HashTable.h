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
    void init(hashCode hashCodeF, equals equalsF) {
        this->hashCode_ = hashCodeF;
        this->equals_ = equalsF;
    }

public:
    HashTable() {
        this->init([](K k) { return k % 32; }, [](K k1, K k2) { return k1 == k2; });
    }

    HashTable(hashCode hashCodeF) {
        this->init(hashCodeF, [](K k1, K k2) { return k1 == k2; });
    }

    HashTable(hashCode hashCodeF, equals equalsF) {
        this->init(hashCodeF, equalsF);
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

    bool contains(K key) {
        int len = get<int>(key, 0, [](int c, V value) {});
        return len > 0;
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
        this->table.template forEach<int>(0, [](int c, LinkedList<Entry<K, V>> *list) {
            if (list == 0) {
                return;
            }
            delete list;
        });
        this->table.clear();
    }

    template <typename C>
    void forEach(C c, void (*consumer)(C, K, V)) {
        struct Params {
            void (*consumer_)(C, K, V);
            C c;
            Params(C c) : c(c) {
            }
        } p(c);
        p.consumer_ = consumer;

        for (int i = 0; i < table.len(); i++) {
            LinkedList<Entry<K, V>> *list = table.get(i);
            if (list == 0) {
                continue;
            }
            list->template forEach<Params *>(&p, [](Params *pp, Entry<K, V> entry) {
                pp->consumer_(pp->c, entry.k, entry.v);
            });
        }
    }

    int remove(K key) {
        return this->remove<int>(0, key, [](int, K, V) {});
    }
    template <typename C>
    int remove(C c, K key, void (*consumer)(C, K, V)) {
        int code = hashCode_(key);

        LinkedList<Entry<K, V>> *list = table.get(code, 0);
        if (list == 0) {
            return false;
        }

        struct Params {
            equals equals_;
            K key;
            C c;
            void (*consumer)(C, K, V);
        };

        Params p;
        p.c = c;
        p.equals_ = this->equals_;
        p.key = key;
        p.consumer = consumer;

        int len = list->template remove<Params *>(
            &p, //
            [](Params *p, Entry<K, V> e1) {
                return p->equals_(e1.k, p->key);
            },
            [](Params *p, Entry<K, V> e1) {
                p->consumer(p->c, e1.k, e1.v);
            }

        );

        return len;
    }

    String &operator>>(String &str) {
        str << "{";

        this->template forEach<String &>(str, [](String &str, K k, V v) {
            str << k << ":" << v << ",";
        });

        return str << "}";
    }
};
} // namespace  a8::util
