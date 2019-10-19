#include "myClass.h"

#include <iostream>

void MyClass::Show()
{
    cout << "this is myclass object\n";
}

MyClass *MyClass::New()
{
    return new MyClass;
}

MyClass::MyClass()
{
    printf( "(%s, %d): this is MyClass constructor!\n", __FUNCTION__, __LINE__ );
}
