#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string.h>
using namespace std;

char *itoa(long long  a){
    bool negtive = false;
    if(a < 0){
        a = -a;
        negtive = true;
    }
    if(a == 0) return "0";
    char *str = new char[64];
    int index = 0;
    while(a){
        str[index++] = a%10+'0';
        a = a/10;
    }
    for(int i=0; i<index/2; i++){
        char tmp = str[i];
        str[i] = str[index-1-i];
        str[index-1-i] = tmp;
    }
    if(negtive){
        memmove(str+1,str,index);
        str[0] = '-';
    }
    return str;
}
int main()
{
    long long a;

    while (cin>>a) {
        cout<<itoa(a)<<endl;
    }

    return 0;
}
