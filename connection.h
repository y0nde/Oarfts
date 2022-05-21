#pragma once
int getServerSock(short port);
int acceptSock(int listenfd);
int getClientSock(char* ip, short port);
