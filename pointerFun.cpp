#include <stdlib.h>
#include <vector>
#include <set>
#include <list>
#include <string>
using namespace std;

void intPrint(void *param){
    printf("int is %d\n",*(int*)param);
}

void doublePrint(void *param){
    printf("double is %lf\n",*(double *)param);
}

void (*pFun)(void *);

int main()
{
    int a = 12;
    double aa = 12.34;
    pFun = intPrint;
    pFun(&a);
    pFun = doublePrint;
    pFun(&aa);
    return 0;
}

/*
typedef void (*pFun)(void *);

int main()
{
    int a = 12;
    double aa = 12.34;
    pFun pf;
    pf = intPrint;
    pf(&a);
    pf = doublePrint;
    pf(&aa);
    return 0;
}
*/
