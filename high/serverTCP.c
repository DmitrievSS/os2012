#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include <map>
#include <string>
#define BUF_LEN 256
#define TABLE_SIZE 1000
#define ADDRESS "127.0.0.1"
#define PORT 30000
using namespace std;
struct Node{
    struct ServiceInformation serviceInformation;
    std::vector<char*> value;
};
struct ServiceInformation{//all service information sends by  UDP
    int key;
    int collision;//collision = -1 already changed = 1 ready to change = 0
    int transaktionNumber;//to kill cycles
    int size;//
    int portFrom;
}
struct ServiceInformation checkServiseInformation(char* str){
    char* pch = strtok(str," ");
    int key = atoi(pch);
    pch = strtok(Null," ");
    int collision = atoi(pch); 
    pch = strtok(Null," ");
    int transaktionNumber = atoi(pch); 
    pch = strtok(Null," ");
    int size = atoi(pch); 
    pch = strtok(Null," ");
    int portFrom = atoi(pch); 
    struct ServiceInformation serviceinformation;
    serviceInformation.key = key;
    serviceInformation.collision = collision;
    serviceInformation.transaktionNumber = transaktionNumber;
    serviceInformation.size = size;
    serviceInformation.portFrom = portFrom;
    return serviceInformation;
}
void read_all(int fd, char* str, size_t len) {
    ssize_t bytes = 0;
    while ((bytes = read(fd, str + bytes, len - bytes)) > 0);
}
int checkCollsion(vector<struct ServiceInformation> servinf, int key){
    int count = 0;
    for (int i = 0; i < servinf.size(); i++){
        if (count >1) 
            return 1;
        if (servinf[i].key == key)
            count++;
    }
    return 0;
}

int checkHistory(vector<struct ServiceInformation> history, struct ServiceInformation transaktion){
    for (int i = 0; i < history.size(); i++){
        if (history[i].transaktion == transaktion.transaktion)
            return 1;
    }
    return 0;
}

int main(int argc, char** argv){//
    int n = argc;
    char* buf;//buffer
    int sock;//socket for service information
    int count; 
    std::vector <struct ServiceInformation>  history;
    int sockout[n];
    std::vector <struct Node> table(TABLE_SIZE);//data
    struct hostent* host = gethostbyname(ADDRESS);
    struct sockaddr_in saddr;
    struct sockaddr_in saddrout[n];
    struct pollfd* fds= (struct pollfd*) malloc((2) * sizeof(struct pollfd));
    std::vector <struct pollfd> fdsTCP;
    std::vector <struct ServiceInformation> servInf;
    sock = socket (AF_INET, SOCK_DGRAM, 0);   
    if (sock == -1){
        printf( "socket() error: %s\n",strerror(errno));
        return 1;
    }
    buf = (char *) malloc(BUF_LEN);
    if (buf == NULL){
        printf("malloc() error: %s\n", strerror(errno));
        return 1;
    }
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(atoi(argv[1]));
    saddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr * ) &saddr, sizeof(saddr))==-1){
        printf("bind() error   %s\n", strerror(errno));
        return 1;
    } 
    fds[0].fd = sock;
    fds[0].events = POLLIN;
    fds[1].fd = 0;//bind stdin
    fds[1].events = POLLIN;
    int portCount = 0;   
    while(1){
        int ready = poll(fds, argc-1, -1);
        if (ready <  0){
            printf("error %i", errno);
            break;
        }
        if (ready == 0){
            printf ("timeout");
            break;
        }
        if (ready > 0){
            if((count = recvfrom(fds[0].fd, buf, BUF_LEN-1,0, NULL, NULL)) ==-1){
                printf("recvdrom() error: %s\n",strerror(errno));
            }
            struct ServiceInformation tmp=parseBuf(buf);//get information about udp connection
            history.push_back(tmp);
            servInf.peush_back(tmp);
            int newPort = PORT;
            if (!checkHistory(history,tmp.serviceInformation){
                int tmpsocket = socket(AF_INET, SOCK_DGRAM, 0);//create a socket to sent working port
                struct sockaddr_in saddr;
                saddr.sin_family = AF_INET;
                saddr.sin_port = htons(tmp.portFrom));
                saddr.sin_addr = *(struct in_addr*)host->h_addr_list[0];
                newPort = PORT+tmp.portFrom;//create new port
                portCount++;//num of working ports
                char tmpBuf[15];
                if (bind(newTCPsock, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
                    fprintf(stderr, "bind() error\n");
                    return 1;
                }
                sprintf(tmpBUF, "%d", PORT+tmp.portFrom);
                sendto(tmpsocket, tmpBuf,strlen(tmpBuf),0,(struct sockaddr *) &saddr, sizeof(struct sockaddr_in));//sending
            }
            int newTCPsock = socket (AF_INET, SOCK_STREAM, 0);   
            struct sockaddr_in saddr;
            saddrout[i].sin_family = AF_INET;
            saddrout[i].sin_port = htons(newPort);
            saddrout[i].sin_addr = *(struct in_addr*)host->h_addr_list[0]; addr.sin_addr.s_addr = INADDR_ANY;
            if (bind(newTCPsock, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
                fprintf(stderr, "bind() error\n");
                return 1;
            }
            pollfd newpoll;
            newpoll.fd=newTCPsock;
            newpoll.events=POLLIN
            fdsTCP.push_back(newPOLL);
            int readyTCP = poll(fdsTCP.data(),argc-1,-1);//ckeck every tcp connection
            if (readyTCP <  0){
                printf("error %i", errno);
                break;
            }
            if (readyTCP == 0){
                printf ("timeout");
                break;
            }
            for(int i = 0; i < fdsTCP.size(); i++){
                if (fdsTCP[i].revents == POLLIN && !checkcollision(servinf[i].key)){
                    readall(fdsTCP[i].fd,buf,1);
                    table[servInf[i].key].value.push_back(buf);
                    if (table[servInf[i].key].value.size() >= servinf[i].size){
                        fdsTCP.erase(i);
                        table[servInf[i].key].serviceInformation().size=servinf[i].size;
                        table[servInf[i].key].serviceInformation().portFrom=servinf[i].portFrom;
                        table[servInf[i].key].serviceInformation().collision=servinf[i].collision;
                        table[servInf[i].key].serviceInformation().key=servinf[i].key;
                        table[servInf[i].key].serviceInformation().transaktionNumber=servinf[i].transaktionNumber;
                        servinf.erase(i);
                    }
                }
            }
/*
                    
        }
        if (fds[0].revents ==  POLLIN){
             struct Node tmp = parseBuf(buf);
             printf("key : %i\n value : %s\n",tmp.key, tmp.value);
             for (int j = 0; j <n-1; j++){   
                 sendto(sockout[j], tmp.value,strlen(tmp.value),0,(struct sockaddr *) &saddrout[j], sizeof(struct sockaddr_in));
             }
             if (table[tmp.key].collision == 1)
                table[tmp.key].collision = -1;
             if (table[tmp.key].collision == 0){
                table[tmp.key].key = tmp.key;
                table[tmp.key].value = tmp.value ;    
                table[tmp.key].collision = tmp.collision;
             }
             n--;
        }
        
        if (fds[1].revents==POLLIN){
            if(!(read(0,buf,100))){
                printf("recvdrom() error^ %s\n%s\n",strerror(errno),buf);
            }
            struct Node tmp = parseBuf(buf);
            for (int j = 0; j <n-1; j++){   
                sendto(sockout[j], tmp.value,strlen(tmp.value),0,(struct sockaddr *) &saddrout[j], sizeof(struct sockaddr_in));
            }
        }
    } 
    return 0;
}
