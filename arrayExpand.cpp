#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;


int main()
{
    // orgin array.
    int *p = new int[10];
    int i;
    for(i=0; i<10; i++){
        p[i] = i+1;
    }

    // expand capacity of the array.
    int *old = p;
    p = new int[20];
    memmove(p,old,10*sizeof(int));  //memmove is recommended. It allowes overlap.
    for(i=10;i<20;i++){
        p[i] = i+1;
    }
    delete [] old;

    // show new array.
    for(i=0; i<20; i++){
        cout<<p[i]<<" ";
    }   cout<<endl;
    delete [] p;
    return 0;
}
