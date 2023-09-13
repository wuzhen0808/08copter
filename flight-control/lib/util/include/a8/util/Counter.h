#ifndef Counter__
#define Counter__

namespace a8 {
namespace util {

class Counter {
public:
    Counter();
    ~Counter();
    Counter* add();
    Counter * minus();
    int get();    
private:
    int value;
};
} // namespace util

} // namespace a8

#endif