#pragma once
#include "a8/util/Buffer.h"

namespace a8::util {

template <typename T>
class LinkedList {
    template <typename E>
    struct Node {
        E ele;
        Node<E> *pre;
        Node<E> *next;
    };

    Node<T> *head;

    template <typename C>
    int findNode(C context, bool (*finder)(C, T)) {
        return findNode<C>(this->head->next, this->head, context, finder, [](C context, Node<T> *node) {});
    }

    template <typename C>
    int findNode(C context, bool (*finder)(C, T), void (*consumer)(C, Node<T> *)) {
        return findNode<C>(this->head->next, this->head, context, finder, consumer);
    }

    template <typename C>
    int findNode(Node<T> *from, Node<T> *to, C context, bool (*finder)(C, T), void (*consumer)(C, Node<T> *)) {
        Node<T> *node = from;
        int len = 0;
        while (node != to) {
            if (finder(context, node->ele)) {
                consumer(context, node);
                len++;
            }
            node = node->next;
        }

        return len;
    }

    void insert(Node<T> *node, T ele) {
        Node<T> *node2 = new Node<T>();
        node2->ele = ele;
        Node<T> *node3 = node->next;
        node->next = node2;
        node2->next = node3;
        node3->pre = node2;
        node2->pre = node;
    }

    Node<T> *tailNode() {
        return this->head->pre;
    }

public:
    LinkedList() {
        this->head = new Node<T>();
        this->head->next = this->head;
        this->head->pre = this->head;
    }

    void add(T ele) {
        insert(this->tailNode(), ele);
    }

    T *first() {
        if (this->isEmpty()) {
            return 0;
        }
        return &this->head->next;
    }

    T *last() {
        if (this->isEmpty()) {
            return 0;
        }
        return &this->head->pre;
    }

    void clear() {
        return clear<int>(0, [](int ctx, T ele) {});
    }

    template <typename C>
    void clear(C context, void (*free)(C, T)) {
        Node<T> *node = this->head->next;
        while (node != this->head) {
            Node<T> *node2 = node->next;
            free(context, node->ele);
            delete node;
            node = node2;
        }
    }

    bool isEmpty() {
        return this->head->next == this->head;
    }

    template <typename C>
    void replaceOrAdd(C context, T ele, bool (*finder)(C, T)) {
        if (this->replace<C>(context, ele, finder)) {
            return;
        }
        this->add(ele);
    }

    template <typename C>
    bool replace(C context, T ele, bool (*finder)(C, T)) {
        struct Params {
            C context_;
            T ele_;
            bool (*finder_)(C, T);
        };

        Params p;
        p.context_ = context;
        p.ele_ = ele;
        p.finder_ = finder;

        return findNode<Params *>(
            &p,                                                            //
            [](Params *p, T ele) { return p->finder_(p->context_, ele); }, //
            [](Params *p, Node<T> *node) {
                node->ele = p->ele_;
            } //
        );
    }

    template <typename C>
    int find(C context, bool (*finder)(C, T), void (*consume)(C context, T ele)) {
        struct Params {
            C context_;
            bool (*finder_)(C, T);
            void (*consume_)(C, T);
        };

        Params p;
        p.context_ = context;
        p.finder_ = finder;
        p.consume_ = consume;

        int len = findNode<Params *>(
            &p,                                                            //
            [](Params *p, T ele) { return p->finder_(p->context_, ele); }, //
            [](Params *p, Node<T> *node) {
                p->consume_(p->context_, node->ele);
            });

        return len;
    }

    bool contains(T ele, bool (*equals)(T e1, T e2)) {
        struct Params {
            T ele_;
            bool (*equals_)(T e1, T e2);
        };

        Params p;
        p.ele_ = ele;
        p.equals_ = equals;

        int found = this->findNode(&p, [](Params *p, T e2) {
            return p->equals_(p->ele_, e2);
        });
        return found > 0;
    }

    template <typename C>
    int remove(C context, bool (*finder)(C, T), void (*consumer)(C, T)) {
        struct Params {
            C context_;
            bool (*finder_)(C, T);
            void (*consumer_)(C, T);
            Node<T> **toBeRemoved = 0;
            int len = 0;
            int cap = 0;
        };
        Params p;
        p.context_ = context;
        p.finder_ = finder;
        p.consumer_ = consumer;

        int len = findNode<Params *>(
            &p, //
            [](Params *p, T ele) {
                return p->finder_(p->context_, ele);
            }, //
            [](Params *p, Node<T> *node) {
                Lang::append(p->toBeRemoved, p->len, p->cap, 16, 0, &node, 0, 1);
            } //
        );

        for (int i = 0; i < p.len; i++) {
            Node<T> *node = p.toBeRemoved[i];
            T ele = node->ele;
            node->pre->next = node->next;
            node->next->pre = node->pre;
            delete node;
            consumer(context, ele);
        }

        if (p.toBeRemoved != 0) {
            delete[] p.toBeRemoved;
        }
        return len;
    }
};
} // namespace  a8::util
