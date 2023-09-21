
#include <functional>
namespace a8::util {

class Finally {
    std::function<void(void)> func;

public:
    Finally(const std::function<void(void)> &func) : func(func) {}
    ~Finally() { func(); }
};

} // namespace a8::util