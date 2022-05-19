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
#define PATH_MAX 256

void disconnect(int fd){
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

int errordisconnect(int listenfd,int clientfd){
    if(clientfd > 0){
        disconnect(clientfd);
    }
    disconnect(listenfd);
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv){
    int rc, tmp;
    int listenfd, clientfd;
    struct sockaddr_in servaddr, cliaddr;
    int yes = 1;

    //ソケット生成
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0){
        printf("create sock fail\n");
        errordisconnect(listenfd,0);
    }

    //ソケット設定
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

    //アドレス生成
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(8080);

    //ポート接続
    rc = bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(rc < 0){
        printf("bind fail\n");
        errordisconnect(listenfd, 0);
    }

    //接続待ち
    rc = listen(listenfd, 5);
    if(rc < 0){
        printf("listen fail\n");
        errordisconnect(listenfd, 0);
    }

    //クライアントソケット生成
    tmp = sizeof(cliaddr);
    clientfd = accept(listenfd, (struct sockaddr*)&cliaddr, (socklen_t*)&tmp);
    if(rc < 0){
        printf("accept fail\n");
        errordisconnect(listenfd, clientfd);
    }

    /*コネクション処理*/
    Request req;
    Response res;
    struct stat st;

    //Requestの受信
    rc = recvRequest(clientfd, &req);
    if(rc < 0){
        printf("recv fail\n");
        errordisconnect(listenfd, clientfd);
    }

    //Request構造体の出力
    printRequest(&req);

    printf("\n");

    //Responseの送信
    res.type = YES;
    rc = sendResponse(clientfd, &res);
    if(rc < 0){
        printf("send fail\n");
        errordisconnect(listenfd, clientfd);
    }

    //statの受信
    rc = recvstat(clientfd, &st); 
    if(rc < 0){
        printf("recv fail\n");
        errordisconnect(listenfd, clientfd);
    }

    //statの出力
    printstat(&st);

    //FileDataの受信
    FileData data;
    rc = recvFileData(clientfd, &data);
    if(rc < 0){
        printf("recv fail\n");
        errordisconnect(listenfd, clientfd);
    }
    printFileData(&data);

    close(clientfd);
    close(listenfd);
    return 0;
}
