#ifndef NUMBER_H
#define NUMBER_H

#define LEN 1000010
enum BOOL{False,True};
typedef enum BOOL bool;
bool num_vis[LEN];
int prime_cnt;
int prime_number[LEN/10];
void get_prime();
#endif //NUMBER_H
