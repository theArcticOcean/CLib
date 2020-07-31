#include <iostream>

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkPointPicker.h>
#include <vector>

using namespace std;

#define vtkSPtr vtkSmartPointer
#define vtkSPtrNew(Var, Type) vtkSPtr<Type> Var = vtkSPtr<Type>::New();

int main()
{
    vtkActor *actor0 = vtkActor::New();
    vtkSPtr<vtkActor> actor1 = actor0;
    vtkSPtr<vtkActor> actor2 = actor0;
    vtkActor *actor3 = actor0;

    auto refCount = actor0->GetReferenceCount();
    cout << "refCount: " << refCount << endl;

    vtkSPtrNew( renderer0, vtkRenderer );
    renderer0->AddActor( actor1 );
    cout << "ptr: " << renderer0.Get() << endl;

    auto *ptr = vtkActor::New();
    actor0->AddConsumer( ptr );
    cout << "ptr: " << ptr << endl;
    auto consCount = actor0->GetNumberOfConsumers();
    cout << "NumberOfConsumers: " << consCount << endl;

    // ======== start to remove actor0 ========
    int numberOfConsumer = actor0->GetNumberOfConsumers();
    std::vector<vtkObject*> consumers;
    for ( int i = 0; i < numberOfConsumer; ++i )
    {
        vtkObject *consumer = actor0->GetConsumer(i);
        consumers.push_back( consumer );
    }
    for ( auto consumer: consumers )
    {
        cout << "consumer: " << consumer << endl;
        vtkRenderer* renderer = dynamic_cast<vtkRenderer*>( consumer );
        if ( renderer )
        {
            renderer->RemoveActor( actor0 );
        }
        else
        {
            actor0->RemoveConsumer( consumer );
        }
        cout << "GetNumberOfConsumers: " << actor0->GetNumberOfConsumers() << endl;
    }

    actor0->Delete();
    actor0 = nullptr;
    // ======== finished: remove actor0 ========
    return 0;
}
