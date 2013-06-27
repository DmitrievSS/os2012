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
#include <vector>
#define BUF_LEN 256
#define TABLE_SIZE 1000
#define ADDRESS "127.0.0.1"
#define PORT 30000
using namespace std;
struct ServiceInformation{//all service information sends by  UDP
    int key;
    int collision;//collision = -1 already changed = 1 ready to change = 0
    int transaktionNumber;//to kill cycles
    int size;//
    int portFrom;
};
struct Node{
    struct ServiceInformation serviceInformation;
    std::vector <char*> value;
};

struct ServiceInformation checkServiseInformation(char* str, int port){
    printf("str - %s\n", str);
    char* pch = strtok(str," ");
    printf("key - %s\n", pch);
    int key = atoi(pch);
    pch = strtok(NULL," ");
    printf("collision - %s\n", pch);
    int collision = atoi(pch); 
    pch = strtok(NULL," ");
    printf("transaction num- %s\n", pch);
    int transaktionNumber = atoi(pch); 
    pch = strtok(NULL," ");
    printf("size %s\n", pch);
    int size = atoi(pch); 
    int portFrom = port; 
    struct ServiceInformation serviceInformation;
    serviceInformation.key = key;
    serviceInformation.collision = collision;
    serviceInformation.transaktionNumber = transaktionNumber;
    serviceInformation.size = size;
    serviceInformation.portFrom = portFrom;
    return serviceInformation;
}
void read_all(int fd, char* str, size_t len) {
    ssize_t bytes = 0;
    while ((bytes = read(fd, str + bytes, len - bytes)) > 0){
        printf("read_all %s\n",str);
    }
}
int checkCollsion(vector<struct ServiceInformation> servinf, int key){
    int count = 0;
    for (int i = 0; i < servinf.size(); i++){
        if (count >1){
           printf("collsion"); 
           return 1;
        }
        if (servinf[i].key == key)
            count++;
    }
    return 0;
}
string strarrToStr(vector<char*>& data){
    string tmp;
    for(int i = 0; i < data.size(); ++i){
        tmp += data[i];
    }
    return tmp;
}
/*void VectorToString (vector<char*> vect, char* c){
    int l = 0;
    for( int i = 0 ; i < vect.size(); i++){
        l = strlen(vect[i]);
    }
    printf("long %i\n", l);
    c =(char*) malloc(l);
    int k = 0;
    for (int i = 0; i < vect.size(); i++){
        for(int j = 0; j < strlen(vect[i]); j++){
            char* ch = vect[i];
            c[k]=ch[j];
            printf("%c",c[k]);
            k++;
        }
    }
}*/
int checkHistory(vector<struct ServiceInformation> history, struct ServiceInformation transaktion){
    for (int i = 0; i < history.size(); i++){
        if (history[i].transaktionNumber == transaktion.transaktionNumber)
            return 1;
    }
    return 0;
}
void write_all(int fd,const char* str, size_t len) {
    ssize_t bytes = 0;
    while ((bytes = write(fd, str + bytes, len - bytes)) > 0);
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
    int sockTCP = socket(AF_INET, SOCK_DGRAM, 0);//socket for getting inf about tcp port
    if (sock == -1){
        printf( "sock() error: %s\n",strerror(errno));
        return 1;
    }
    if (sockTCP == -1){
        printf( "sockTCP() error: %s\n",strerror(errno));
        return 1;
    }
    buf = (char *) malloc(BUF_LEN);
    if (buf == NULL){
        printf("malloc() error: %s\n", strerror(errno));
        return 1;
    }
    printf("1\n");
    saddr.sin_family = AF_INET;
    printf("2");
    saddr.sin_port = htons(atoi(argv[1]));
    saddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr * ) &saddr, sizeof(saddr))==-1){
        printf("bind() error   %s\n", strerror(errno));
        return 1;
    } 
    for (int i = 0; i < n-1; i++){
        sockout[i] = socket (AF_INET, SOCK_DGRAM, 0);   
        if (i ==( n-2)) continue;
        saddrout[i].sin_family = AF_INET;
        saddrout[i].sin_port = htons(atoi(argv[i+2]));
        saddrout[i].sin_addr = *(struct in_addr*)host->h_addr_list[0];
        if (connect(sockout[i], (struct sockaddr * ) &saddrout[i], sizeof(saddrout[i]))==-1){
            printf("bind() error   %s\n", strerror(errno));
            return 1;
        } 
    }
    fds[0].fd = sock;
    fds[0].events = POLLIN;
    fds[1].fd = 0;//bind stdin
    fds[1].events = POLLIN;
    printf("2\n");
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
            printf("ready to %i\n", fds[1].revents);
            if( fds[1].revents == POLLIN){
                printf("send like klient\n");
               // if((count = recvfrom(fds[1].fd, buf, BUF_LEN-1,0, NULL, NULL)) ==-1){
                 //   printf("recvdrom() error: %s\n",strerror(errno));
               // }
                read(0,buf,BUF_LEN);
                char *buf1,* tt = (char*) malloc (BUF_LEN);
                memcpy(tt,buf,BUF_LEN);
                struct ServiceInformation tmp=checkServiseInformation(buf,atoi(argv[1]));//get information about udp connection
                printf("stdin : %s, %i\n",tt, strlen(buf));
                history.push_back(tmp);
                servInf.push_back(tmp);
                printf("servinf checked\n");
                read(6,buf1,tmp.size);
                printf("buf1 %s\n",buf1);
                table[tmp.key].value.push_back(buf1);
                printf("changed table\n");
                table[tmp.key].serviceInformation.size=tmp.size;
                table[tmp.key].serviceInformation.portFrom=tmp.portFrom;
                table[tmp.key].serviceInformation.collision=tmp.collision;
                table[tmp.key].serviceInformation.key=tmp.key;
                table[tmp.key].serviceInformation.transaktionNumber=tmp.transaktionNumber;
                 for (int j = 0; j <n-1; j++){   
                   //  string str = strarrToStr(table[tmp.key].value);
                    // printf("c - %s\n",str.c_str());
                     sendto(sockout[j],tt,strlen(tt),0,(struct sockaddr *)
                             &saddrout[j], sizeof(struct sockaddr_in));//send servifdormation about new data
                     printf("waiting for PORT\n");
                     count = 1;
                     while (1){
                         printf("wait\n");
                     //   if((count = recvfrom(sockTCP, buf, BUF_LEN-1,0, NULL, NULL)) ==-1){//get port to tcp connection
                       //     printf("recvdrom() error: %s\n",strerror(errno));
                       // }
                        read(sockTCP, buf, BUF_LEN-1);
                        printf("buf %s\n", buf);
                        if( count >0){
                            printf("got Port\n");
                            int tcpPort = atoi(buf);
                            int tmps = socket (AF_INET, SOCK_STREAM, 0);   
                            struct sockaddr_in saddrtmp;
                            saddrtmp.sin_family = AF_INET;
                            saddrtmp.sin_port = htons(atoi(argv[2+j]));
                            saddrtmp.sin_addr = *(struct in_addr*)host->h_addr_list[0]; 
                            if (connect(tmps, (struct sockaddr*)&saddrtmp, sizeof(saddrtmp)) == -1) {
                                fprintf(stderr, "connect() error\n");
                                return 1;
                            }
                            write_all(tmps,strarrToStr(table[tmp.key].value).c_str(),table[tmp.key].serviceInformation.size);
                            printf("sent\n");
                            break;
                        }
                     }
                }

            }
            if( fds[0].revents == POLLIN){
                printf("get inf from server\n");
                /*if((count = recvfrom(fds[0].fd, buf, BUF_LEN-1,0, NULL, NULL)) ==-1){
                    printf("recvdrom() error: %s\n",strerror(errno));
                }*/
                read(fds[0].fd,buf,BUF_LEN);
                printf("buf,%s %i\n",buf, count);
                struct ServiceInformation tmp=checkServiseInformation(buf,atoi(argv[1]));//get information about udp connection
                servInf.push_back(tmp);
                int newPort = PORT;
                printf("connction pepl %i\n", history.size());
                if (!checkHistory(history,tmp)){
                    history.push_back(tmp);
                    printf("start connction peple\n");
                    int tmpsocket = socket(AF_INET, SOCK_DGRAM, 0);//create a socket to sent working port
                    struct sockaddr_in saddr;
                    saddr.sin_family = AF_INET;
                    saddr.sin_port = htons(tmp.portFrom);
                    saddr.sin_addr = *(struct in_addr*)host->h_addr_list[0];
                    newPort = PORT+tmp.portFrom;//create new port
                    printf("new PORT created ,%i\n",newPort);
                    portCount++;//num of working ports
                    char tmpBuf[15];
                    if (connect(tmpsocket, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
                        printf("bind() error: %s\n",strerror(errno));
                        return 1;
                    }
                    char tmpBUF[15];
                    sprintf(tmpBUF, "%d", PORT+tmp.portFrom);
                    printf("PORT %s\n",tmpBUF);
                    sendto(tmpsocket, tmpBuf,strlen(tmpBuf),0,(struct sockaddr *) &saddr, sizeof(struct sockaddr_in));//sending
                }
                int newTCPsock = socket (AF_INET, SOCK_STREAM, 0);   
                struct sockaddr_in saddr;
                saddr.sin_family = AF_INET;
                saddr.sin_port = htons(newPort);
                saddr.sin_addr = *(struct in_addr*)host->h_addr_list[0];
                if (bind(newTCPsock, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
                    fprintf(stderr, "bind() error\n");
                    return 1;
                }
                struct pollfd newpoll;
                newpoll.fd=newTCPsock;
                newpoll.events=POLLIN;
                fdsTCP.push_back(newpoll);
                printf("readyTCP\n");
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
                        printf("there is  collision %i %i \n", fdsTCP[i].revents, !checkCollsion(servInf,servInf[i].key));
                    if ((fdsTCP[i].revents & POLLIN) && !checkCollsion(servInf,servInf[i].key)){
                        printf("there is no collision\n");
                        read(fdsTCP[i].fd,buf,100);
                        printf("get from tcp %s\n", buf);
                        table[servInf[i].key].value.push_back(buf);
                        if (table[servInf[i].key].value.size() >= servInf[i].size){
                            fdsTCP.erase(fdsTCP.begin()+i);
                            table[servInf[i].key].serviceInformation.size=servInf[i].size;
                            table[servInf[i].key].serviceInformation.portFrom=servInf[i].portFrom;
                            table[servInf[i].key].serviceInformation.collision=servInf[i].collision;
                            table[servInf[i].key].serviceInformation.key=servInf[i].key;
                            table[servInf[i].key].serviceInformation.transaktionNumber=servInf[i].transaktionNumber;
                            int key = servInf[i].key;                         
                            servInf.erase(servInf.begin()+i);
                             
                            for (int j = 0; j <n-1; j++){   
                                 sendto(sockout[j],&table[key].value ,table[key].serviceInformation.size,0,(struct sockaddr *)
                                         &saddrout[j], sizeof(struct sockaddr_in));//send servindormation about new data
                                 count = 1;
                                 while (count > 0){
                                    if((count = recvfrom(sockTCP, buf, BUF_LEN-1,0, NULL, NULL)) ==-1){//get port to tcp connection
                                        printf("recvdrom() error: %s\n",strerror(errno));
                                    }
                                    int tcpPort = atoi(buf);
                                    int tmp = socket (AF_INET, SOCK_STREAM, 0);   
                                    struct sockaddr_in saddrtmp;
                                    saddrtmp.sin_family = AF_INET;
                                    saddrtmp.sin_port = htons(newPort);
                                    saddrtmp.sin_addr = *(struct in_addr*)host->h_addr_list[0];
                                    if (connect(tmp, (struct sockaddr*)&saddrtmp, sizeof(saddrtmp)) == -1) {
                                        fprintf(stderr, "connect() error\n");
                                        return 1;
                                    }
                                    write_all(sock,strarrToStr(table[i].value).c_str(),table[i].serviceInformation.size);
                                 }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
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
}*/
