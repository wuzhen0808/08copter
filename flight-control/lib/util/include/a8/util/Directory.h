#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Debug.h"
#include "a8/util/HashTable.h"
#include "a8/util/PushBackReader.h"
#include "a8/util/Reader.h"
#include "a8/util/String.h"

#define A8_DIRECTORY_DEBUG (0)
namespace a8::util {
template <typename T>
class Directory {

public:
    using releaseAttribute = void (*)(void *);
    class Attribute {
    public:
        void *value;
        releaseAttribute release;
        Attribute(void *value, releaseAttribute release) {
            this->value = value;
            this->release = release;
        }
        ~Attribute() {
            this->release(this->value);
        }
        void set(void *value, releaseAttribute release) {
            this->release(this->value);
            this->value = value;
            this->release = release;
        }
    };

public:
    using titleF = String (*)(Directory<T> *);
    const static int TAG_KEY_FOCUS = 1;

protected:
    Buffer<Directory<T> *> children;
    String name;
    Directory<T> *parent;
    T element;
    HashTable<int, char> tags;
    HashTable<int, Attribute *> attributes;
    void init(Directory<T> *parent, String name, T ele) {
        this->name = name;
        this->parent = parent;
        this->element = ele;
    }

public:
    titleF title = [](Directory<T> *dir) { return dir->getName(); };

public:
    Directory(T ele) {
        init(0, "Unknown", ele);
    }

    Directory(String name, T ele) {
        init(0, name, ele);
    }

    Directory(Directory<T> *parent, String name, T ele) {
        init(parent, name, ele);
    }

    ~Directory() {

        if (this->parent != 0) {
            // remove from parent.
            this->parent->children.removeEle(this);
            this->parent = 0;
        }
        // delete all children.
        for (int i = 0; i < this->children.len(); i++) {
            Directory *child = this->children.get(i,0);
            delete child;
        }
        // delete attribute
        this->attributes.template forEach<int>(0, [](int c, int k, Attribute *att) {
            delete att;
        });
        this->attributes.clear();
    }

    void setAttribute(int key, void *value, releaseAttribute release) {
        Attribute *att = this->attributes.get(key, 0);
        if (att == 0) {
            att = new Attribute(value, release);
            this->attributes.set(key, att);
        } else {
            att->set(value, release);
        }
    }

    template <typename X>
    X getAttribute(X def) {
        return this->getAttribute(0, def);
    }
    template <typename X>
    X getAttribute(int key, X def) {
        Attribute *att = this->attributes.get(key, 0);
        if (att == 0 || att->value == 0) {
            return def;
        }
        return static_cast<X>(att->value);
    }

    int getTotalChildren() {
        return this->children.len();
    }
    String getTitle() {
        return this->title(this);
    }

    void tag(int key, char v) {
        this->tags.set(key, v);
    }

    char getTag(int key, char def) {
        return this->tags.get(key, def);
    }

    bool isRoot() {
        return this->parent == 0;
    }
    Directory<T> *getParent() {
        return this->parent;
    }
    String getName() {
        return name;
    }
    Directory<T> *getFirstChild() {
        Directory<T> * ele = 0;
        this->children.getFirst(ele);
        return ele;        
    }

    Directory<T> *getLastChild() {
        Directory<T> * ele = 0;
        this->children.getLast(ele);
        return ele;        
    }

    Buffer<Directory<T> *> getChildren() {
        return children;
    }
    void setElement(T element) {
        this->element = element;
    }

    T getElement() {
        return this->element;
    }

    Directory<T> *addChild(String name, T ele) {
        Directory<T> *child = new Directory<T>(this, name, ele);
        this->children.append(child);
        return child;
    }

    int getChildIndex(Directory<T> *child) {
        return this->children.indexOf(child);
    }

    Directory<T> *getChildIfHas(int idx) {
        return children.get(idx, 0);
    }

    Directory<T> *findChild(Buffer<String> path) {
        return findChild(path, 0);
    }

    Directory<T> *findChild(Buffer<String> path, int subPathIdx) {
        if (subPathIdx == path.len()) {
            return 0;
        }
        if (subPathIdx == path.len() - 1) {
            return findChild(path.get(subPathIdx, ""));
        }
        String childName = path.get(subPathIdx,"");
        Directory *child = findChild(childName);
        if (child == 0) {
            return 0;
        }

        return child->findChild(path, subPathIdx + 1);
    }

    Directory<T> *findChild(String name) {
        Directory<T> *ret = 0;
        for (int i = 0; i < this->children.len(); i++) {
            Directory<T> *mt = children.get(i);
            if (mt->name == name) {
                ret = mt;
                break;
            }
        }
        return ret;
    }
};

} // namespace a8::util