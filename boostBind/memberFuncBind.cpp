/**********************************************************************************************
*
*   Filename: memberFuncBind.cpp
*
*   Author: theArcticOcean - wei_yang1994@163.com
*
*   Create: 2018-06-10 23:09:25
**********************************************************************************************/

#include <boost/bind.hpp>
#include <vector>
#include <iostream>
using namespace boost;

struct point
{
    int x, y;
    point(int a = 0, int b = 0):x(a),y(b){}
    void print()
    {   std::cout << "(" << x << "," << y << ")\n";  }
};

int main()
{
    std::vector<int> nums{3, 4, 2, 8, 15, 267};
    // 声明Lambda表达式: capture list] (params list) mutable exception-> return type { function body }
    auto print = [](const int& n) { std::cout << " " << n; };
    // for_each
    std::for_each(nums.begin(), nums.end(), print);
    std::cout << '\n';
 
    // bind
    std::vector<point> v(10);
    std::for_each(v.begin(), v.end(), bind(&point::print, _1));  // bind, second param is object pointer
    return 0;
}
