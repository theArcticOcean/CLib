/**********************************************************************************************
*
*   Filename: binary_function.cpp
*
*   Author: theArcticOcean - wei_yang1994@163.com
*
**********************************************************************************************/

#include <iostream>
#include <Set>
using namespace std;

class IString
{
public:
    IString( const char *_str)
    {
        strcpy( str, _str );
    }
    char *get()
    {
        return str;
    }
    friend ostream& operator << ( ostream &out, IString obj );
    
private:
    char str[105];
};

ostream& operator << ( ostream &out, IString obj )
{
    out << obj.get();
    return out;
}

// inherts binary_function and define operator() to compare two IString.
struct IStringCompare: std::binary_function<IString, IString, bool>
{
    bool operator()( IString s1, IString s2 ) const
    {
        char tmp1[105];
        char tmp2[105];
        strcpy( tmp1, s1.get() );
        strcpy( tmp2, s2.get() );
        transform( tmp1, tmp1 + 105, tmp1, ::tolower );
        transform( tmp2, tmp2 + 105, tmp2, ::tolower );
        cout << tmp1 << ", " << tmp2 << endl;
        return strcmp( tmp1, tmp2 );
    }
};

int main(int argc, char *argv[])
{
    set<IString, IStringCompare> mySet;
    mySet.insert( IString( "HELLO" ) );
    mySet.insert( IString( "hello" ) );
    cout << "mySet.size is " << mySet.size() << endl;   // mySet.size is 1
    return 0;
}
