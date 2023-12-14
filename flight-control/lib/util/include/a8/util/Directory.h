#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/PushBackReader.h"
#include "a8/util/Reader.h"
#include "a8/util/String.h"

namespace a8::util {

template <typename T>
class Directory {
    using memoF = String (*)(T);

protected:
    Buffer<Directory<T> *> children;
    String name;
    Directory<T> *parent;
    T element;
    void init(Directory<T> *parent, String name, T ele) {
        this->name = name;
        this->parent = parent;
        this->element = ele;
    }

public:
    memoF memo = [](T) { return String(); };

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
            Directory *child = this->children.get(i);
            delete child;
        }
    }

    String getMemo() {
        return this->memo(this->element);
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
            return findChild(path.get(subPathIdx));
        }
        String childName = path.get(subPathIdx);
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

template <typename T>
class DirectoryCursor {
protected:
    Directory<T> *root_;
    Directory<T> *tree;
    int indexAsChild = -1; // unknown index.
    Directory<T> *mark_ = 0;
    void to(Directory<T> *dir, int idx) {
        this->tree = dir;
        this->indexAsChild = idx;
    }

public:
    DirectoryCursor(Directory<T> *tree) {
        this->tree = tree;
        this->root_ = tree;
    }

    Directory<T> *get() {
        return this->tree;
    }

    void to(Directory<T> *dir) {
        to(dir, -1);
    }

    void root() {
        this->tree = root_;
    }

    void mark() {
        this->mark_ = tree;
    }
    bool unMark() {
        if (this->mark_ == 0) {
            return false;
        }
        to(mark_);
        mark_ = 0;
        return true;
    }
    bool down() {
        return next(1);
    }

    bool up() {
        return next(-1);
    }

    bool left() {
        return left(1);
    }

    bool left(int offset) {
        Directory<T> *next = this->tree;
        for (int i = 0; i < offset; i++) {
            Directory<T> *parent = next->getParent();
            if (parent == 0) {
                return false;
            }
            next = parent;
        }
        to(next);
        return true;
    }

    bool right() {
        Directory<T> *next = this->tree->getChildIfHas(0);
        if (next == 0) {
            return false;
        }
        to(next, 0);
        return true;
    }

    bool next(int offset) {
        Directory<T> *parent = this->tree->getParent();
        if (parent == 0) {
            return false;
        }
        if (this->indexAsChild == -1) {
            this->indexAsChild = parent->getChildIndex(this->tree);
        }
        int idx = this->indexAsChild + offset;
        Directory<T> *next = parent->getChildIfHas(idx);
        if (next == 0) { // parent has no such child.
            return false;
        }
        to(next, idx);
        return true;
    }

    bool top() {
        int offset = 0;
        while (next(-1)) {
            offset++;
        }
        return offset > 0;
    }

    bool bottom() {
        int offset = 0;
        while (next(1)) {
            offset++;
        }
        return offset > 0;
    }
};

template <typename C, typename T>
class DirectoryNavigator : public DirectoryCursor<T> {
    using onDirectoryChangedF = void (*)(C, DirectoryNavigator<C, T> *);
    using onDirectoryEnterF = void (*)(C, DirectoryNavigator<C, T> *);
    using onLeftFailureF = void (*)(C, DirectoryNavigator<C, T> *);
    using onRightFailureF = void (*)(C, DirectoryNavigator<C, T> *);
    using onUpFailureF = void (*)(C, DirectoryNavigator<C, T> *);
    using onDownFailureF = void (*)(C, DirectoryNavigator<C, T> *);

protected:
    Reader *reader;
    Output *out;
    bool running = true;
    int ret = 0;

public:
    onDirectoryChangedF onDirectoryChanged = [](C, DirectoryNavigator<C, T> *) {

    };
    onDirectoryEnterF onDirectoryEnter = [](C, DirectoryNavigator<C, T> *) {

    };

    onLeftFailureF onLeftFailure = [](C, DirectoryNavigator<C, T> *) {

    };
    onRightFailureF onRightFailure = [](C, DirectoryNavigator<C, T> *) {

    };
    onUpFailureF onUpFailure = [](C, DirectoryNavigator<C, T> *) {

    };
    onDownFailureF onDownFailure = [](C, DirectoryNavigator<C, T> *) {

    };

public:
    DirectoryNavigator(Reader *reader, Output *out, Directory<T> *tree) : DirectoryCursor<T>(tree) {
        this->reader = reader;
        this->out = out;
    }
    void stop(int ret) {
        this->ret = ret;
        running = false;
    }
    int run(C c) {
        print();
        while (running) {
            int ret = doRead(c);
            if (ret < 0) {
                this->stop(ret); // stop with error code.
            }
        }
        return this->ret;
    }
    void print() {
        out->println(String() << ">>print:" << (this->tree == 0));
        Directory<T> *parent = this->tree->getParent();
        out->println("+------ directory tree -------+");
        if (parent == 0) {
            doPrint(this->tree);
        } else {
            Buffer<Directory<T> *> list = parent->getChildren();
            for (int i = 0; i < list.len(); i++) {
                doPrint(list.get(i));
            }
        }
        out->println("+-----------------------------+");
    }

    void doPrint(Directory<T> *dir) {
        out->println(String() << (dir == this->tree ? "[*]" : "[ ]") << dir->getName() << "(" << dir->getMemo() << ")");
    }
    void log(String msg) {
        out->println(msg);
    }

    int doRead(C c) {
        char ch;
        int ret = this->reader->read(ch);
        if (ret < 0) {
            return -1; // end of interaction.
        }
        log(String() << "ch,i:" << (int)ch << ",ch:" << ch);
        bool changed = false;

        switch (ch) {
        case 10: {
            this->onDirectoryEnter(c, this);
        } break;
        case 65: // up
        {
            log("up");
            changed = this->up();
            if (!changed) {
                this->onUpFailure(c, this);
            }
        } break;
        case 66: // down
        {
            log("down");
            changed = this->down();
            if (!changed) {
                this->onDownFailure(c, this);
            }
        } break;

        case 68: // left
        {
            log("left");

            changed = this->left();
            if (!changed) {
                this->onLeftFailure(c, this);
            }
        } break;
        case 67: // right
        {
            log("right");

            changed = this->right();
            if (!changed) {
                this->onRightFailure(c, this);
            }
        } break;
        default:
            break;
        }

        this->print();
        if (changed) {
            this->onDirectoryChanged(c, this);
        }

        log(String() << "dir changed:" << changed);
        return 1;
    }
};
} // namespace a8::util