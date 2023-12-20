#pragma once
namespace a8::fc::collect {
class Context {
private:
    Context(const Context &);
    Context operator=(const Context &);

public:
    Writer *writer;
};

} // namespace a8::fc::collect