#include "readlines.h"
#include "unistd.h"
#include "stdio.h"

int main(int argc, char** argv){
    if(argc < 2){
        printf("Max size not given");
        return -1;
    }
    struct RL* rl = rl_open(1,atoi(argv[1]));
    char* buf = malloc(rl->max_size);
    int bytes_read = 0;
    do{
        bytes_read = rl_readline(rl, buf, rl_max_size(rl));
        if(bytes_read > 0)
            write(0, buf, bytes_read);
    } while(bytes_read != 0);
    return 0;
}