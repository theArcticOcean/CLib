#include <iostream>

using namespace std;

class Cat
{

public:
    Cat(int a){ num = a; }

//Private effect: 'Cat::Cat' : cannot access private member declared in class 'Cat'
private:

    //delete effect: 'Cat::Cat(const Cat &)' : attempting to reference a deleted function
    Cat(const Cat &) = delete;
    Cat &operator = ( const Cat & ) = delete;

    int num;
};

int main(int argc, char *argv[])
{
    Cat c1( 0 );
 //   Cat c2( c1 );
 //   Cat c2 = c1;
    return 0;
}
