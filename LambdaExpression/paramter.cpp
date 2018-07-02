#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

/*
Lambda Expression's Paramters:
1. 参数列表中不能有默认参数
2. 不支持可变参数
3. 所有参数必须有参数名
 */

int main()
{
	vector<int> vec;
	for( int i=0; i<10; ++i )
	{
		vec.push_back( i+1 );
	}
	auto fun = [](vector<int> &V) { 
		for( int i=0; i<V.size(); ++i )
		{
			V[i] += 10;
			cout << V[i] << " ";
		}
		cout << endl;
	};

	fun( vec );

	for( int i=0; i<vec.size(); ++i )
	{
		cout << vec[i] << " ";
	}
	cout << endl;

	return 0;
}
/*
out:
11 12 13 14 15 16 17 18 19 20
11 12 13 14 15 16 17 18 19 20
 */
