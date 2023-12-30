#pragma once
namespace a8::util {
template <typename T>
class MovingAvg {
    int cap;
    int width;
    int head;
    T *queue;
    T sum;

public:
    MovingAvg(int cap) {
        this->cap = cap;
        this->queue = new T[cap];
        this->width = 0;
        this->head = 0;
        this->sum = T(0);
        for (int i = 0; i < cap; i++) {
            this->queue[i] = T(0);
        }
    }
    ~MovingAvg() {
        delete[] queue;
    }
    
    T update(T nValue) {
        if (width < cap) {
            width++;
        }
        sum = sum - queue[head] + nValue;
        queue[head++] = nValue;
        if (head == cap) {
            head = 0;
        }
        return sum / width;
    }
};
} // namespace a8::util