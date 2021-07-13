#include "Base.h"

#include <iostream>
#include <vtkObject.h>

Base::Base()
{
    vtkObject *obj = vtkObject::New();
    obj->PrintSelf( std::cout, vtkIndent() );
    obj->Delete();
}

void Base::Say()
{
    std::cout << "hello~" << std::endl;
}
