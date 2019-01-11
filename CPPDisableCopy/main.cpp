#include <iostream>

using namespace std;

class Cat
{

public:
    Cat(){ }

//Private effect: 'Cat::Cat' : cannot access private member declared in class 'Cat'
private:
    //delete effect: 'Cat::Cat(const Cat &)' : attempting to reference a deleted function
    Cat(const Cat &) = delete;
    Cat &operator = ( const Cat & ) = delete;

    void test(){  }
};

class Driver: public Cat
{
public:
    Driver(){ }
};

int main(int argc, char *argv[])
{
    Cat c1;
    Driver d1;
    //    Driver d2 = d1;  //'Cat::Cat' : cannot access private member declared in class 'Cat'
    return 0;
}
