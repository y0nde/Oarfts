/*サーバー側のオペレーション*/
//コネクションごとにセッションを持つ必要がある。
//*オープンファイルのリスト
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "list.h"
#include "filetransfer.h"

//server
typedef struct ServerSession {
    int clientfd;
    List* openfiles;
} ServerSession;

ServerSession* newServerSession();
void freeServerSession(ServerSession* session);
int handleOpenRequest(ServerSession* session, OpenRequest* req);
int handleCloseRequest(ServerSession* session, CloseRequest* req);
int handleReadRequest(ServerSession* session, ReadRequest* req);
int handleWriteRequest(ServerSession* session, WriteRequest* req);
int handleStatRequest(ServerSession* session, StatRequest* req);
int handleReaddirRequest(ServerSession* session, ReaddirRequest* req);

//client
typedef struct ClientSession {
    char* ip;
    short port;
    int serverfd;
} ClientSession;

ClientSession* newClientSession(char* ip, short port);
void freeClientSession(ClientSession* session);
int openRemoteFile(char* path, int mode);
int closeRemoteFile(char* path, int fd);
int readRemoteFile(char* path, int fd, char* buf, int offset, int size);
int writeRemoteFile(char* path, int fd, char* buf, int offset, int size);
int statRemoteFile(char* path, struct stat *stbuf);
int readdirRemoteFile(char* path, int fd, List* stats);






















/*クライアント側のオペレーション*/
