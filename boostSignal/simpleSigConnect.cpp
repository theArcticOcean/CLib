/**********************************************************************************************
*
*      Filename: simpleSigConnect.cpp
*
*        Author: WeiStephen - wei_yang1994@163.com
*
*        Create: 2018-06-10 19:51:11
**********************************************************************************************/

#include <iostream>
#include <boost/signals2.hpp>
using namespace std;

void slot1()
{
    cout << "slot1\n";
}

void slot2()
{
    cout << "slot2\n";
}

int main()
{
    boost::signals2::signal<void ()> sig;
    sig.connect( &slot1 );
    sig.connect( &slot2 );

    sig();   // operator () emit signal
    return 0;
}
/*
slot1
slot2
*/

