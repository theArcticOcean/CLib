#pragma once

#include <vtkObjectFactory.h>

class MyFactory: public vtkObjectFactory
{
public:
    static MyFactory *New();
    virtual const char* GetVTKSourceVersion();
    virtual const char* GetDescription();
protected:
    MyFactory();
};
