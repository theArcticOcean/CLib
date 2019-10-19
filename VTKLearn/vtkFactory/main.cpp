#include <iostream>
#include <vtkSmartPointer.h>
#include <vtkCollection.h>

#include "myFactory.h"
#include "myClass.h"

using namespace std;

int main()
{    
    vtkSmartPointer<MyFactory> factory = vtkSmartPointer<MyFactory>::New();
    vtkObjectFactory::RegisterFactory ( factory );
    cout << factory->GetVTKSourceVersion() << endl;
    cout << factory->GetDescription() << endl;
    cout << factory->GetReferenceCount() << endl;

//    vtkSmartPointer<vtkCollection> collect = vtkSmartPointer<vtkCollection>::New();
//    factory->CreateAllInstance( "MyClass", collect );
    MyClass *obj = dynamic_cast<MyClass *>( factory->CreateInstance( "MyClass" ) );
    cout << (void *)obj << endl;

    /*cout << (void *)collect->GetItemAsObject( 0 ) << endl;
    MyClass *obj = dynamic_cast<MyClass *>( collect->GetItemAsObject( 0 ) );
    obj->Show(); */
    return 0;
}
/*
vtk version 8.1.1
this is a test factory
2
*/
