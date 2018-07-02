#include <iostream>
#include <algorithm>
using namespace std;

/*
[capture list] (params list) mutable exception-> return type { function body }
各项具体含义如下
capture list：捕获外部变量列表
params list：形参列表
mutable指示符：用来说用是否可以修改捕获的变量
exception：异常设定
return type：返回类型
function body：函数体
*/

int main(int argc, char *argv[])
{

    int a[ 5 ] = { 12, 78, 32, 44, 100 };
    sort( a, a+5, []( int v1, int v2 ) -> bool { return v1 < v2; } );
    for( int i=0; i<5; i++ )
    {
        cout << a[i] << " ";
    }
    cout << endl;
    return 0;
}
