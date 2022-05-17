CC = gcc
CFLAG = -ggdb

all: simpleFTSserver.c simpleFTSclient.c
	${CC} ${CFLAG} -o server simpleFTSserver.c
	${CC} ${CFLAG} -o client simpleFTSclient.c

