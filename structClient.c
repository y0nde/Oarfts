#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>

#define CHUNK_SIZE 1024

int errordisconnect(int sockfd){
    close(sockfd);
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv){
    int rc;
    int sockfd;
    int offset, size, nread, rcv_size;
    struct sockaddr_in servaddr;
    char buf[CHUNK_SIZE];
   
    //引数チェック
    if(argc < 2){
        printf("client [path]\n");
        errordisconnect(sockfd);
    }

    //ソケット生成
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("create sock fail\n");
        errordisconnect(sockfd);
    }

    //アドレス生成
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(8080);

    //接続
    rc = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(rc < 0){
        printf("connect fail\n");
        errordisconnect(sockfd);
    }
    
    //サーバーにパスを送信
    rc = send(sockfd, argv[1], strlen(argv[1]), 0);
    if(rc < 0){
        printf("send fail\n");
        errordisconnect(sockfd);
    }

    while(1){
        //バッファの初期化
        bzero(buf, CHUNK_SIZE);

        //受信の取得
        rcv_size = recv(sockfd, buf, CHUNK_SIZE, 0);
        if(rcv_size <= 0){
            printf("\n[recv error]\n");
            errordisconnect(sockfd);
        }

        //受信データの出力
        buf[rcv_size] = '\0';
        printf("%s", buf);

        //ループ条件
        if(rcv_size < CHUNK_SIZE){
            break;
        }
    }

    return 0;
}
