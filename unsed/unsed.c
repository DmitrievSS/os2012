#include "unistd.h"
#include "stdio.h"
#include "readlines.h"
#include <pcre.h>
#include <string.h>
#include <stdlib.h>

#define STR_LENGTH 256
#define BUFFER_LEN STR_LENGTH * 4

void match_all_in_str(pcre* re, char* str, int str_len, int greedy, char* repl, int repl_len){
    int count = 0;
    int ovector[30];
    int offset = 0;
    char* buf;
    int strpos = 0;
    int bufpos = 0;
    buf = malloc(BUFFER_LEN);
    memset(buf, 0, BUFFER_LEN);
    while((offset < str_len) && (count = pcre_exec(re,NULL, str, str_len, offset, NULL, ovector, 30) >= 0)){
        int i;
        strncat(&buf[bufpos], &str[strpos], ovector[0] - strpos);
        bufpos += ovector[0] - strpos;
        strpos = ovector[1];
        strncat(&buf[bufpos], repl, repl_len);
        bufpos += repl_len;
       // for (i = 0; i < count; ++i){
       //     printf("%d %d\n", ovector[2*i], ovector[2*i+1]); 
       // }
       // printf("TMP(%d,%d): %s\n", bufpos, strpos, buf);
        offset = ovector[1];
        if(!greedy)
            return;
    }
    strncat(&buf[bufpos], &str[strpos], str_len - strpos+1);
    //printf("%s\n", buf);
    write(0, buf, strlen(buf));

}

int main(int argc, char** argv){
    if(argc < 2){
        printf("Pattern not found");
        return -1;
    }
    struct RL* rl = rl_open(1, STR_LENGTH);
    char* buf = malloc(rl->max_size);
    int bytes_read = 0;

    strtok(argv[1], "/"); 
    char* pattern_tmp = strtok(NULL, "/");//skip s/
    char* pattern = malloc(strlen(pattern_tmp)+1);
    strcpy(pattern, pattern_tmp);
    pattern_tmp = strtok(NULL, "/");
    char* repl = malloc(strlen(pattern_tmp)+1);
    strcpy(repl, pattern_tmp);
    int greedy = (strtok(NULL, "/") != NULL);
    //printf("%s\n%s\n%d\n", pattern, repl, greedy);

    pcre* re;
    int options = PCRE_UTF8;
    const char* error;
    int erroffset;
    re = pcre_compile(pattern, options, &error, &erroffset, NULL);

    if(!re){
        printf("RE compilation failed");
        return -1;
    } 

    do{
        bytes_read = rl_readline(rl, buf, rl_max_size(rl));
        if(bytes_read > 0){
           // write(0, buf, bytes_read);
            match_all_in_str(re, buf, bytes_read, greedy, repl, strlen(repl));
        }

    }while(bytes_read != 0);

    return 0;
}