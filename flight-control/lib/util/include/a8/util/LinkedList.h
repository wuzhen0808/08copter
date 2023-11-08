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
    int findNode(C context, bool (*finder)(C, T), void (*consume)(C, Node<T> *)) {
        Node<T> **buffer = new Node<T> *[0];
        int len = findNode<C>(context, finder, buffer);
        for (int i = 0; i < len; i++) {
            consume(context, buffer[i]);
        }
        delete buffer;
        return len;
    }

    template <typename C>
    int findNode(C context, bool (*finder)(C, T)) {
        Node<T> **buffer = 0;
        return findNode<C>(this->head->next, this->head, finder, buffer);
    }

    template <typename C>
    int findNode(C context, bool (*finder)(C, T), Node<T> **&buffer) {
        return findNode<C>(this->head->next, this->head, context, finder, buffer);
    }

    template <typename C>
    int findNode(Node<T> *from, Node<T> *to, C context, bool (*finder)(C, T), Node<T> **&buffer) {
        Node<T> *node = from;
        int len = 0;
        int cap = 0;
        bool useBuffer = buffer != 0;
        while (node != to) {
            if (finder(context, node->ele)) {
                if (useBuffer) {
                    Lang::append<Node<T> *>(buffer, len, cap, 16, 0, &node, 0, 1);
                }
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
        Node<T> *node = this->head->next;
        while (node != this->head) {
            Node<T> *node2 = node->next;
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
            &p, [](Params *p, T ele) { return p->finder_(p->context_, ele); },
            [](Params *p, Node<T> *node) {
                node->ele = p->ele_;
            });
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
    int remove(C context, bool (*finder)(C, T), T *&removedBuffer) {
        bool useBuffer = removedBuffer != 0;
        Node<T> **nodeBuffer = useBuffer ? new Node<T> *[0] : 0;

        int len = findNode<C>(this->head->next, this->head, context, finder, nodeBuffer);

        bool appendBuffer = removedBuffer != 0;
        int len2 = 0;
        int cap2 = 0;
        for (int i = 0; i < len; i++) {
            Node<T> *node = nodeBuffer[i];
            node->pre->next = node->next;
            node->next->pre = node->pre;
            if (appendBuffer) {
                Lang::append<T>(removedBuffer, len2, cap2, 16, 0, &node->ele, 0, 1);
            }
            delete node;
        }

        if (useBuffer) {
            delete[] nodeBuffer;
        }
        return len;
    }
};
} // namespace  a8::util
