#include "../pthTextRW.h"
#include "../prime_stack.h"
#include <stdio.h>

int main(){
	prime_stack_cnt = 6;
	prime_stack[0] = 2;
	prime_stack[1] = 3;
	prime_stack[2] = 5;
	prime_stack[3] = 7;
	prime_stack[4] = 11;
	prime_stack[5] = 13;
	pthTextRW_run(NULL);
	prime_write();
	prime_index_read();
	return 0;	
}
