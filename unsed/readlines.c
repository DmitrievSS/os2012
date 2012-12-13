#include "readlines.h"




struct RL* rl_open(int x, size_t max_size){
    struct RL* _rl = malloc(sizeof(struct RL));
    assert(_rl != NULL);
    _rl->fd = x;
    _rl->max_size = max_size;
    return _rl;
}

int rl_close(struct RL* x){
    assert(x != NULL);
    int ret = close(x->fd);
    free(x);
    return ret;
}

size_t rl_max_size(struct RL* x){
    assert(x != NULL);
    return x->max_size;
}



int rl_readline(struct RL* rl, char* buf, size_t buf_size){
    assert(rl != NULL);
    size_t pos = 0;
    memset(buf,0,buf_size);
    if( (pos = read(rl->fd, buf, sizeof(char))) == 0)
        return 0;
    while (buf[pos-1] != '\n' && pos < buf_size+1 && pos < rl->max_size)
    {
        pos += read(rl->fd, &buf[pos], sizeof(char));
        if(pos == 0)
            return 0;
    } 
    if(buf[pos-1] == '\n')
        return pos;
    if(pos == rl->max_size+1)
        return -3;
    if(pos == buf_size){
        char tmp;
        do{
            int has_read = read(rl->fd, &tmp, sizeof(char));
            if(has_read == 0)
                return 0;
        } while(tmp != '\n');
        return -2;
    }
    return -1337;
}