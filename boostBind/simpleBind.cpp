/**********************************************************************************************
*
*   Filename: simpleBind.cpp
*
*   Author: theArcticOcean - wei_yang1994@163.com
*
*   Create: 2018-06-10 22:35:05
**********************************************************************************************/

#include <boost/bind.hpp>
#include <iostream>

using namespace std;

int f( int a, int b )
{
    return a+b;
}

int g( int a, int b, int c)
{
    return a+b+c;
}

int main()
{
    cout << boost::bind(f, 1, 2)() << endl;
    cout << boost::bind(g, 1, 2, 3)() << endl;
    return 0;
}
/*
绑 定 完成 后， bind 会 返回 一个 函数 对象， 它 内部 保存 了 f 的 拷贝， 具有 operator（）， 返回 值 类型 被 自动 推导 为 f 的 返回 值 类型。 在 发生 调用 时， 这个 函数 对象 将把 之前 存储 的 参数 转发 给 f 完成 调用。 例如， 如果 有一个 函数 func， 它的 形式 是: func(a1, a2) 那么， 它将 等价 于 一个 具 有无 参 operator（） 的 bind 函数 对象 调用：bind(func,a1,a2)()

罗剑锋. Boost程序库完全开发指南——深入C++“准”标准库（第3版） (Kindle Locations 6249-6254). 电子工业出版社. Kindle Edition. 
*/
