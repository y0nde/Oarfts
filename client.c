#include <stdio.h>
#include "list.h"
#include "session.h"
#include "structCommon.h"

int main(){
    int rc, fd;
    ClientSession* session;

    session = newClientSession("127.0.0.1", 8080);
    if(session == NULL){
        return -1;
    }

    char* path = "./Makefile";
    char* path2 = "./sample.txt";
    char* dir = ".";
    char buf[2048] = {0};
    char* buf2 = "writeRemoteFile-test";

    fd = openRemoteFile(session, path, O_RDWR);
    if(fd < 0){
        return -1;
    }

    rc = readRemoteFile(session, path, fd, buf, 0, 2048);
    if(rc < 0){return -1;}else{printf("%s\n", buf);}

    fd = openRemoteFile(session, path2, O_RDWR);
    if(fd < 0){
        return -1;
    }

    rc = writeRemoteFile(session, path2, fd, buf2, 0, strlen(buf2)+1);
    if(rc < 0){return -1;}else{printf("%s\n", buf);}

    struct stat stbuf;
    List* attrs;

    rc = statRemoteFile(session, path, &stbuf);
    if(rc < 0){puts("error");return -1;}else{printstat(&stbuf);}

    attrs = newList();
    rc = readdirRemoteFile(session, dir, fd, attrs);
    if(rc < 0){puts("error");return -1;}else{printList(attrs, printFileAttr);}
    return 0;

    freeClientSession(session);
}
