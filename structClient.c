#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include "structCommon.h"

#define CHUNK_SIZE 1024

int errordisconnect(int sockfd){
    close(sockfd);
    exit(EXIT_FAILURE);
}

/*接続*/
int stconnect(char* ip, u_int16_t port){
    int rc;
    int sockfd;
    struct sockaddr_in servaddr;
    
    //ソケット生成
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("create sock fail\n");
        errordisconnect(sockfd);
    }

    //アドレス生成
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);

    //接続
    rc = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(rc < 0){
        printf("connect fail\n");
        return -1;
    }

    return sockfd;
}

int main(int argc, char** argv){
    int rc;
    int sockfd;
    int offset, size, nread, rcv_size;
    struct sockaddr_in servaddr;
    char buf[CHUNK_SIZE];
   
    sockfd = stconnect("127.0.0.1", 8080);

    //Requestの作成
    Request req = {0};
    req.type = NONE;
    strcpy(req.path, "/PATH/");

    //サーバーにパスを送信
    rc = sendRequest(sockfd, &req);
    if(rc < 0){
        printf("send fail\n");
        errordisconnect(sockfd);
    }

    //Responseの受信
    Response res = {0};
    rc = recvResponse(sockfd, &res);
    if(rc < 0){
        printf("recv fail\n");
        errordisconnect(sockfd);
    }
    printResponse(&res);

    //statの作成
    struct stat st = {0};
    stat("./structClient.c", &st);
    rc = sendstat(sockfd, &st);
    if(rc < 0){
        printf("send fail\n");
        errordisconnect(sockfd);
    }
    
    //ファイルの送信
    
    //ファイルデータの用意
    FILE* file;
    FileChunk data = {0};

    file = fopen("sample.txt", "r+");
    if(rc < 0){
        printf("open fail\n");
        errordisconnect(sockfd);
    }

    rc = fread(data.data, 1, CHUNK_SIZE, file);
    if(rc < 0){
        printf("read fail\n");
        errordisconnect(sockfd);
    }

    //送信
    data.index = 0;
    rc = sendFileChunk(sockfd, &data);
    if(rc < 0){
        printf("send fail\n");
        errordisconnect(sockfd);
    }

    close(sockfd);
    return 0;
}
