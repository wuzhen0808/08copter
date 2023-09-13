#include "a8/util/Attributes.h"
#define INC (10)
namespace a8 {
namespace util {

Attributes::Attributes() {
}
Attributes::~Attributes() {
    delete this->attributes;
}
void **Attributes::ensureCapacity(int key) {
    if (key >= capacity) {

        void **tmp1 = this->attributes;
        int cap2 = this->capacity + INC;
        void **tmp2 = new void *[cap2];
        for (int i = 0; i < this->capacity; i++) {
            tmp2[i] = tmp1[i];
        }
        delete tmp1;

        this->attributes = tmp2;
        this->capacity = cap2;
    }
    return this->attributes;
}

void *Attributes::get(int key) {
    if (key < 0 || key >= capacity) {
        return 0;
    }
    return ensureCapacity(key)[key];
}
void Attributes::set(int key, void *value) {
    ensureCapacity(key)[key] = value;
}
} // namespace util

} // namespace a8