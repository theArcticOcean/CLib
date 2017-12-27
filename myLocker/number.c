#include "number.h"

typedef long long LL;
void get_prime(){
	LL i;
	LL j;
	for(i=0;i<LEN;i++){
		num_vis[i] = 0;
	}
	prime_cnt = 0;
	for(i=2;i<LEN;i++){
		if(!num_vis[i]){
			prime_number[prime_cnt] = i;
			prime_cnt++;
		}
		for(j=0; j<prime_cnt&&i*prime_number[j]<LEN;j++){
			num_vis[i*prime_number[j]] = 1;
			if(i%prime_number[j] == 0) break;
		}
	}		
}
