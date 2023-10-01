#ifndef Callback__
#define Callback__
namespace a8::util {

template <typename A, typename B>
class Pair {
public:
    A a;
    B b;
    Pair(A a, B b) {
        this->a = a;
        this->b = b;
    }
};

} // namespace a8::util
#endif