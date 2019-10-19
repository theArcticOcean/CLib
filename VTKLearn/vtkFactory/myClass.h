#pragma once

#include <vtkObject.h>

class MyClass: public vtkObject
{
public:
    void Show();
    static MyClass* New();
protected:
    MyClass();
};
