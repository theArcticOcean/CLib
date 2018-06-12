/**********************************************************************************************
*
*   Filename: runtime_error.cpp
*
*   Author: theArcticOcean - wei_yang1994@163.com
*
*   Create: 2018-06-12 22:27:20
**********************************************************************************************/

#include <iostream>
#include <stdexcept>
using namespace std;

int get(int a)
{
    if ( a == 0 ){
        throw runtime_error("a = 0!");
    }

    return a;
}

int main()
{
    cout << "test: " << endl;
    get(0);
/*
test:

This application has requested the Runtime to terminate it in an unusual way.
Please contact the application's support team for more information.
*/

// try
// {
// get(0);
// }
// catch (const exception& e)
// {
// cout << e.what() << endl;
// }

/*
test:
a = 0!
*/
    return 0;
}
