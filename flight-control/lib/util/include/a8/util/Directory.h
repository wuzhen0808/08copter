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
    using titleF = String (*)(Directory<T> *);
    const static int TAG_KEY_FOCUS = 1;

protected:
    Buffer<Directory<T> *> children;
    String name;
    Directory<T> *parent;
    T element;
    HashTable<int, char> tags;
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
            Directory *child = this->children.get(i);
            delete child;
        }
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
        return this->neighbor(1);
    }

    bool up() {
        bool changed = this->neighbor(-1, false);
        if (!changed) {
            Directory<T> *parent = this->tree->getParent();
            if (parent != 0) {
                to(parent, -1);
                changed = true;
            }
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

    bool neighbor(int offset) {
        return neighbor(offset, true);
    }

    bool neighbor(int offset, bool adjustOffset) {
        Directory<T> *parent = this->tree->getParent();
        if (parent == 0) {
            // this is root,no brother.
            return false;
        }
        if (this->indexAsChild == -1) {
            this->indexAsChild = parent->getChildIndex(this->tree);
        }
        int idx = this->indexAsChild + offset;
        Buffer<Directory<T> *> buffer = parent->getChildren();
        if (adjustOffset) {
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
        print(0, 0, path.get(0), path);
        out->println("-----------------------------+");
    }

    void print(int depth, int idx, Directory<T> *dir, Buffer<Directory<T> *> &path) {
        Buffer<Directory<T> *> list = dir->getChildren();
        bool expand = false;
        if (depth < path.len() - 1 && path.get(depth) == dir) {
            // only expand focused directories.
            // do not expand the last one.
            expand = true;
        }
        doPrint(depth, idx, expand, dir); // single print.
        if (expand) {
            // print child if need expand.
            for (int i = 0; i < list.len(); i++) {
                print(depth + 1, i, list.get(i), path);
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