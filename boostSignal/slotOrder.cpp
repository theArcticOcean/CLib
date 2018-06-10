/**********************************************************************************************
*
*      Filename: slotOrder.cpp
*
*        Author: theArcticOcean - wei_yang1994@163.com
*
*        Create: 2018-06-10 19:51:11
**********************************************************************************************/

#include <iostream>
#include <boost/signals2.hpp>
using namespace std;

template<int N>
struct slots
{
    void operator()()
    {
        cout << "slot" << N << " is called." << endl;
    }
};

int main()
{
    boost::signals2::signal<void ()> sig;
    sig.connect( slots<1000>(), boost::signals2::at_back );
    sig.connect( slots<1>(), boost::signals2::at_front );
    
    sig.connect( 5, slots<59>(), boost::signals2::at_back );
    sig.connect( 5, slots<51>(), boost::signals2::at_front );

    sig.connect( 3, slots<39>(), boost::signals2::at_back );
    sig.connect( 3, slots<31>(), boost::signals2::at_front );
    
    sig.connect( 10, slots<100>(), boost::signals2::at_front );

    sig();  // operator () emit signal
    return 0;
}
/*
connect（） 函数 的 另一个 重载 形式 可以 在 连接 时 指定 插槽 所 在的 组 号， 缺省 情况下 组 号 是 int 类型。 组 号 不一定 要从 0 开始 连续 编号， 它 可以 是 任意 的 数值， 离散 的、 负值 都 允许。 如果 在 连接 的 时候 指定 组 号， 那么 每个 编组 的 插槽 将是 又一个 插槽 链 表， 形成 一个 略微 有些 复杂 的 二维 链 表， 它们 的 顺序 规则 如下： ■ 各 编组 的 调用 顺序 由 组 号 从小到大 决定（ 也可 以在 signal 的 第四 个 模板 参数 改变 排序 函数 对象）； ■ 每个 编组 的 插槽 链 表 内部 的 插入 顺序 用 at_ back 和 at_ front 指定； ■ 未被 编组 的 插槽 如果 位置 标志 是 at_ front， 将 在 所有 的 编组 之前 调用； ■ 未被 编组 的 插槽 如果 位置 标志 是 at_ back， 将 在 所有 的 编组 之后 调用。 我们 使用 一个

罗剑锋. Boost程序库完全开发指南——深入C++“准”标准库（第3版） (Kindle Locations 6525-6533). 电子工业出版社. Kindle Edition. 
*/

