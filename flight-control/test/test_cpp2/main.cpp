#include <iostream>

struct A
{
    virtual ~A()
    {
        std::cout << "~A" << std::endl;
    }
    int memberA;
};

struct B
{
    virtual ~B()
    {
        std::cout << "~B" << std::endl;
    }
    int memberB;
};

struct AB : public A, public B
{
    virtual ~AB()
    {
        std::cout << "~AB" << std::endl;
    }
};


int main()
{
    AB* ab1 = new AB();
    AB* ab2 = new AB();

    A* a = ab1;
    B* b = ab2;

    delete a;
    delete b;
}