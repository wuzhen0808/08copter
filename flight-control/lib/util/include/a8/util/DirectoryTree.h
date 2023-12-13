#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/String.h"

namespace a8::util {

class DirectoryTree {
protected:
    Buffer<DirectoryTree *> children;
    String name;
    DirectoryTree *parent;

    void *element;

public:
    DirectoryTree(DirectoryTree *parent, String name) {
        this->name = name;
        this->parent = parent;
        this->element = 0;
    }

    ~DirectoryTree() {

        if (this->parent != 0) {
            // remove from parent.
            this->parent->children.removeEle(this);
            this->parent = 0;
        }
        // delete all children.
        for (int i = 0; i < this->children.len(); i++) {
            DirectoryTree *child = this->children.get(i);
            delete child;
        }
    }
    void setElement(void *element) {
        this->element = element;
    }

    void *getElement() {
        return this->element;
    }

    virtual DirectoryTree *newDirectoryTree(DirectoryTree *parent, String name) = 0;

    DirectoryTree *addChild(String name) {
        DirectoryTree *child = newDirectoryTree(this, name);
        this->children.append(child);
        return child;
    }

    int getChildIndex(DirectoryTree *child) {
        return this->children.indexOf(child);
    }

    DirectoryTree *findChild(Buffer<String> path) {
        return findChild(path, 0);
    }

    DirectoryTree *findChild(Buffer<String> path, int subPathIdx) {
        if (subPathIdx == path.len()) {
            return 0;
        }
        if (subPathIdx == path.len() - 1) {
            return findChild(path.get(subPathIdx));
        }
        String childName = path.get(subPathIdx);
        DirectoryTree *child = findChild(childName);
        if (child == 0) {
            return 0;
        }

        return child->findChild(path, subPathIdx + 1);
    }

    DirectoryTree *findChild(String name) {
        DirectoryTree *ret = 0;
        for (int i = 0; i < this->children.len(); i++) {
            DirectoryTree *mt = children.get(i);
            if (mt->name == name) {
                ret = mt;
                break;
            }
        }
        return ret;
    }
};
} // namespace a8::util