/**********************************************************************************************
*
*   Filename: ostringstream.cpp
*
*   Author: theArcticOcean - wei_yang1994@163.com
*
**********************************************************************************************/
#include <iostream>
#include <sstream>

using namespace std;

int main()
{
    ostringstream oss;
    oss << "hello ";
    cout << oss.str().c_str();
    oss.seekp( 0, ios_base::beg);
    oss << "world ";
    cout << oss.str().c_str();
    return 0;
}
/*
hello world
*/
