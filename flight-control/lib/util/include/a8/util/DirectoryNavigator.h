#pragma once

#include "a8/util/Buffer.h"
#include "a8/util/Stack.h"
#include "a8/util/Debug.h"
#include "a8/util/Directory.h"
#include "a8/util/HashTable.h"
#include "a8/util/Reader.h"
#include "a8/util/String.h"

#define A8_DIRECTORY_DEBUG (0)
namespace a8::util {

template <typename T>
class DirectoryCursor {

protected:
    Directory<T> *root_;
    Directory<T> *tree;
    int indexAsChild = -1; // unknown index.
    Stack<Directory<T> *> marks;
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

    Buffer<Directory<T> *> getPath() {
        Buffer<Directory<T> *> path;
        Directory<T> *dir = this->get();
        while (dir != 0) {
            path.append(dir);
            dir = dir->getParent();
        }
        path.reverse();
        return path;
    }

    void to(Directory<T> *dir) {
        to(dir, -1);
    }

    void root() {
        this->tree = root_;
    }

    void mark() {
        this->marks.push(this->tree);
    }
    bool unMark() {
        Directory<T> *top = this->marks.pop(0);
        if (top == 0) {
            return false;
        }
        to(top);
        return true;
    }

    virtual bool down() {
        bool changed = this->neighbor(1, false);
        if (!changed) {
            changed = parentNeighbor(1);
        }
        return changed;
    }

    virtual bool up() {
        bool changed = this->neighbor(-1, false);
        if (!changed) {
            changed = parent();
        }
        return changed;
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

    bool parentNeighbor(int offset) {

        Directory<T> *parent = this->tree->getParent();
        if (parent == 0) {
            return false;
        }

        this->mark();
        this->to(parent, -1);
        bool changed = this->neighbor(offset);
        if (!changed) {
            // rollback.
            this->unMark();
        }
        return changed;
    }

    bool parent() {
        return left();
    }

    bool lastNeighbor() {
        Directory<T> *parent = this->tree->getParent();
        if (parent == 0) {
            return false; // no neighbor.
        }
        Directory<T> *child = parent->getLastChild();
        if (child == 0) {
            return false;
        }
        to(child);
        return true;
    }

    bool firstNeighbor() {
        Directory<T> *parent = this->tree->getParent();
        if (parent == 0) {
            return false; // no neighbor.
        }
        Directory<T> *child = parent->getFirstChild();
        if (child == 0) {
            return false;
        }
        to(child);
        return true;
    }

    int ensureIndexAsChild() {
        if (this->indexAsChild == -1) {
            Directory<T> *parent = this->tree->getParent();
            if (parent != 0) {
                this->indexAsChild = parent->getChildIndex(this->tree);
            }
        }
        return this->indexAsChild;
    }

    bool neighbor(int offset) {
        return neighbor(offset, true);
    }

    bool neighbor(int offset, bool allowLoop) {
        Directory<T> *parent = this->tree->getParent();
        if (parent == 0) {
            // this is root,no brother.
            return false;
        }

        int idx = ensureIndexAsChild() + offset;
        Buffer<Directory<T> *> buffer = parent->getChildren();
        if (allowLoop) {
            if (idx < 0) {
                idx += buffer.len();
            }

            if (idx >= buffer.len()) {
                idx = idx - buffer.len();
            }
        }

        Directory<T> *next = parent->getChildIfHas(idx);
        if (next == 0) { // parent has no such child.
            return false;
        }
        to(next, idx);
        return true;
    }

    bool top() {
        int offset = 0;
        while (neighbor(-1)) {
            offset++;
        }
        return offset > 0;
    }

    bool bottom() {
        int offset = 0;
        while (neighbor(1)) {
            offset++;
        }
        return offset > 0;
    }
};

template <typename C, typename T>
class DirectoryNavigator : public DirectoryCursor<T> {
    using keyHandler = void (*)(C, DirectoryNavigator<C, T> *);
    using onDirectoryChangedF = void (*)(C, DirectoryNavigator<C, T> *);
    using onDirectoryEnterF = void (*)(C, DirectoryNavigator<C, T> *);

protected:
    Reader *reader;
    Output *out;
    bool running = true;
    int ret = 0;

public:
    char enterKey = 10;
    char upKey = 65;
    char downKey = 66;
    char leftKey = 68;
    char rightKey = 67;
    char endKey = 70;
    char pgUpKey = 53;
    char pgDownKey = 54;
    char homeKey = 72;
    HashTable<char, keyHandler> handlers;

public:
    DirectoryNavigator(Reader *reader, Output *out, Directory<T> *tree) : DirectoryCursor<T>(tree) {
        this->reader = reader;
        this->out = out;

        setHandler(upKey, [](C c, DirectoryNavigator<C, T> *nav) {
            nav->up();
        });

        setHandler(downKey, [](C c, DirectoryNavigator<C, T> *nav) {
            nav->down();
        });
        setHandler(leftKey, [](C c, DirectoryNavigator<C, T> *nav) {
            nav->left();
        });
        setHandler(rightKey, [](C c, DirectoryNavigator<C, T> *nav) {
            nav->right();
        });
        setHandler(enterKey, [](C c, DirectoryNavigator<C, T> *nav) {
            // do nothing.
        });
        setHandler(pgDownKey, [](C c, DirectoryNavigator<C, T> *nav) {
            bool changed = nav->lastNeighbor();
            if (!changed) {
                nav->parentNeighbor(1);
            }
        });

        setHandler(pgUpKey, [](C c, DirectoryNavigator<C, T> *nav) {
            bool changed = nav->firstNeighbor();
            if (!changed) {
                nav->parent();
            }
        });
    }
    void setUpHandler(keyHandler handler) {
        setHandler(upKey, handler);
    }
    void setDownHandler(keyHandler handler) {
        setHandler(downKey, handler);
    }

    void setLeftHandler(keyHandler handler) {
        setHandler(leftKey, handler);
    }
    void setRightHandler(keyHandler handler) {
        setHandler(rightKey, handler);
    }

    void setEnterHandler(keyHandler handler) {
        setHandler(enterKey, handler);
    }

    void setHandler(char key, keyHandler handler) {
        this->handlers.set(key, handler);
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
        A8_OUT_DEBUG(out, String() << ">>print:" << (this->tree == 0));

        Buffer<Directory<T> *> path = this->getPath();
        out->println("+------ Directory ------------");
        print(0, 0, path.get(0, 0), path);
        out->println("-----------------------------+");
    }

    void print(int depth, int idx, Directory<T> *dir, Buffer<Directory<T> *> &path) {
        Buffer<Directory<T> *> list = dir->getChildren();
        bool expand = false;
        if (depth < path.len() - 1 && path.get(depth, 0) == dir) {
            // only expand focused directories.
            // do not expand the last one.
            expand = true;
        }
        doPrint(depth, idx, expand, dir); // single print.
        if (expand) {
            // print child if need expand.
            for (int i = 0; i < list.len(); i++) {
                print(depth + 1, i, list.get(i,0), path);
            }
        }
    }

    void doPrint(int depth, int idx, bool expand, Directory<T> *dir) {
        String line;
        StringUtil::space(line, depth);
        out->println(line
                     << (dir == this->tree ? " o-> " : "     ") // focus.
                     << (dir->getTotalChildren() > 0 ? (expand ? "-" : "+") : " ")
                     << (idx > 9 ? "[" : "[ ") << idx << "]" // index
                     << dir->getTitle())                     // title.
            ;
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
        Directory<T> *dir = this->get();
        keyHandler handler = handlers.get(ch, 0);
        if (handler == 0) {
            // no such key.
        } else {
            handler(c, this);
            Directory<T> *dir2 = this->get();
            if (dir != dir2) { // changed.
            }
            this->print();
        }

        return 1;
    }
};
} // namespace a8::util