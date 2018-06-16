#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define usec 100000

static const char* words = "扣颗纽扣，系上父亲的爱；摸摸脑袋，那是无言的爱；坚定果敢，那是行动的表率；言语少，那是刚柔相济的爱。细节见关爱，父爱暖心怀。祝父亲节日快乐！";

static const char *graph[] = {
"****** ******",
"********** **********",
"************* *************",
"*****************************",
"*****************************",
"*****************************",
"***************************",
"***********************",
"*******************",
"***************",
"***********",
"*******",
"***",
"*"
}; 

int main()
{
    int i, j;
    int len = strlen( words );
    int lineLen = 0;
    int maxLen = 0;
    for( i = 0; i < 14; i++ )
    {
        maxLen = maxLen > strlen( graph[i] ) ? maxLen : strlen( graph[i] );
    }

    for( i = 0 ; i < len; i += 3 ){
        char str[5];
        int ret;
        str[0] = words[i];
        str[1] = words[i+1];
        str[2] = words[i+2];
        str[3] = 0;
        ret = write( STDOUT_FILENO, str, 4 );
        if( ret == -1 ){
            perror( "write " );
            return -1;
        }
        usleep( usec );
        lineLen += 2;
        if( lineLen > 2 * maxLen )
        {
            lineLen = 0;
            puts("");
        }
    }
    puts("");

    for( i = 0; i < 14; i++ )
    {
        int moreSpace = maxLen - strlen( graph[i] );
        while( moreSpace-- ) write( STDOUT_FILENO, " ", 2 );

        write( STDOUT_FILENO, graph[i], strlen( graph[i] ) );
        write( STDOUT_FILENO, graph[i], strlen( graph[i] ) );
        puts("");
        usleep( usec );
    }
    return 0;
}
