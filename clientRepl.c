#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "session.h"
#include "structCommon.h"

int printStatREPL(ClientSession* session, char* path){
    int rc;
    struct stat stbuf;

    rc = statRemoteFile(session, path, &stbuf);
    if(rc < 0){
        printf("stat error\n");
        return -1;
    }

    printstat(&stbuf);
    return 0;
}

void printReadREPL(ClientSession* session, char* path){
    int rc, size, fd;
    struct stat stbuf;
    char res[4] = {0};
    char* buf;

    rc = statRemoteFile(session, path, &stbuf);
    if(rc < 0){
        printf("stat error\n");
        return;
    }
    size = stbuf.st_size;

    if(size > 10000000){
        printf("file size is bigger than 10MB, output? (y/n)\n>> ");
        scanf("%3s", res);
        if(strcmp(res ,"y") == 0 | strcmp(res ,"y") == 0 |strcmp(res ,"Y") == 0 |strcmp(res ,"yes") == 0 ){
            fd = openRemoteFile(session, path, O_RDONLY);
            if(fd < 0){
                printf("open fail\n");
                return;
            }

            buf = malloc(size);
            rc = readRemoteFile(session, path, fd, buf, 0, size);
            if(rc < 0){
                printf("read fail\n");
                return;
            }
            rc = closeRemoteFile(session, path, fd);

            printf("%s\n", buf);
        }
    }else{
        fd = openRemoteFile(session, path, O_RDONLY);
        if(fd < 0){
            printf("open fail\n");
            return;
        }

        buf = malloc(size);
        rc = readRemoteFile(session, path, fd, buf, 0, size);
        if(rc < 0){
            printf("read fail\n");
            return;
        }
        rc = closeRemoteFile(session, path, fd);

        printf("%s\n", buf);
    }
}

int main(){
    int rc, fd;
    ClientSession* session;
    char command[32] = {0};
    char path[256] = {0};

    session = newClientSession("127.0.0.1", 8080);
    if(session == NULL){
        return -1;
    }

    printf("Repl start\n");
    while(1){
        bzero(command, 32);
        bzero(path, 256);
        printf(">> ");
        scanf("%s%s", command, path);
        if(strcmp(command, "STAT") == 0){
            printStatREPL(session, path);
        }else if(strcmp(command, "READ") == 0){
            printReadREPL(session, path);
        }else if(strcmp(command, "EXIT") == 0){
            break;
        }
    }

    freeClientSession(session);
}
