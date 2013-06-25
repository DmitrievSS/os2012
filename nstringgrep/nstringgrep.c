#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int result(char* tempstring, char** words, int n, int length){
    for (int i = 0; i < n; i++){
         //  printf("yes %s %s \n", tempstring, words[i+1]);
           if (strstr(tempstring, words[i+1])!=NULL){
                write(1,tempstring,length);
                return 0;
           }
    }
    return 0;
}
int main(int argc, char** argv){
    int n = argc-1;
    char* buf ;
    char* buffer;
    buf = (char*) malloc(64*1024*sizeof(char));
    buffer = (char*) malloc(64*1024*sizeof(char));
    int fd = 0;
    int l;
    int i = 0;
    while (0<= (l=read(fd, buf,1))){

        if (buf[0]>=32 && buf[0]!=127 && l!=0) {
           // printf("char %c\n", buf[0]);

            buffer[i] = buf[0];   
            i++;
        }
        else{
            buffer[i] = '\n';
            buffer[1+i] = '\0';
            i=i+2;
          //  buffer[i+1]='\0';
           // printf("%i\n", i);
            result(buffer, argv, n, i);

            i = 0;
        }
        if( l == 0) break;
    }
    return 0;
}
