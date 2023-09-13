#ifndef Shared__
#define Shared__
#include "a8/util/Counter.h"
namespace a8 {
namespace util {

template <typename T>
class Shared {
public:
    static const int INC = 10;
    Shared(T *pointer);
    Shared(Shared &shared); // copy constructor
    ~Shared();

    T *get() {
        return pointer;
    }

private:
    Counter *counter;
    T *pointer;
};
// implementation

template <typename T>
Shared<T>::Shared(T *pointer) {
    this->pointer = pointer;
    this->counter = new Counter();
}

template <typename T>
Shared<T>::Shared(Shared<T> &shared) {
    this->pointer = shared.pointer;
    this->counter = shared.counter;
    this->counter->add();
}

template <typename T>
Shared<T>::~Shared() {
    this->counter->minus();
    if (this->counter->get() == 0) {
        delete this->counter;
        delete this->pointer;
    }
}

} // namespace util
} // namespace a8
#endif