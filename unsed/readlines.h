#include "assert.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"

struct RL{
    int fd;
    size_t max_size;
};

struct RL* rl_open(int fd, size_t max_size);
size_t rl_max_size (struct RL *rl);
int rl_close(struct RL *rl);
int rl_readline(struct RL *rl, char * buf, size_t buf_size);