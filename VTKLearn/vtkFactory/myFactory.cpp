#include "myFactory.h"
#include "myClass.h"

#include <vtkVersion.h>

/*
 * shared library implement:
VTK_FACTORY_INTERFACE_IMPLEMENT(MyFactory)
*/

  VTK_CREATE_CREATE_FUNCTION(MyClass)

/*
 *  The method GetVTKSourceVersion() should return VTK_SOURCE_VERSION and
 *  should NOT call vtkVersion::GetVTKSourceVersion().
 *  Because vtkVersion.cpp had been built in installed VTK version.
*/
MyFactory::MyFactory()
{
    this->RegisterOverride( "MyClass", "MyClass1", "MyFactory created drive class", 1, vtkObjectFactoryCreateMyClass );
    cout << "RegisterOverride\n";
}

MyFactory *MyFactory::New()
{
    return new MyFactory;
//    static MyFactory instance;
//    return &instance;
}

const char *MyFactory::GetVTKSourceVersion()
{
    return VTK_SOURCE_VERSION;
}

const char *MyFactory::GetDescription()
{
    char *str = "this is a test factory";
    return str;
}
