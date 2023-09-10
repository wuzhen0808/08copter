#ifndef Buffer__
#define Buffer__
namespace a8 {
namespace util {

template <typename T>
class Buffer {
public:
    Buffer<T>();
    Buffer<T>(int capacity);
    Buffer<T>(T *buffer, int length);
    Buffer<T>(T *buffer, int length, int capacity);
    ~Buffer();
    int getLength();
    T get(int idx);
    T* getAll();
    int remove(int from);
    Buffer<T> *append(T element);
    Buffer<T> *appendAll(T *elements, int length);

private:
    int capacity;
    int length;
    T *buffer;
    void ensureCapacity(int capacity);
};
} // namespace util

} // namespace a8

#endif