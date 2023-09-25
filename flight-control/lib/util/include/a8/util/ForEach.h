#pragma once
#include <list>
namespace a8::util {

class ForEach {
public:
    template <typename T, typename V>
    static bool forEach(T *array, int len, V visitor) {
        for (int i = 0; i < len; i++) {            
            bool interrupt = visitor(array[i]);
            if (interrupt) {
                return true;
            }           
        }
        return false;
    }
};

} // namespace a8::util