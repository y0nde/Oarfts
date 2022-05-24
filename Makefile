CC = gcc
CFLAG = -ggdb

all: struct

simple: 
	${CC} ${CFLAG} -o server simpleFTSserver.c
	${CC} ${CFLAG} -o client simpleFTSclient.c

struct: byteorder misc
	${CC} ${CFLAG} -o structCommon.o -c structCommon.c 
	${CC} ${CFLAG} -o server structServer.c structCommon.o byteorder.o
	${CC} ${CFLAG} -o client structClient.c structCommon.o byteorder.o

byteorder:
	${CC} ${CFLAG} -o byteorder.o -c byteorder.c

connection: 
	${CC} ${CFLAG} -o connection.o -c connection.c 

filetransfer: byteorder struct connection list
	${CC} ${CFLAG} -o filetransfer filetransfer.c  connection.o  structCommon.o byteorder.o list.o

misc:
	echo "hello world.\nThis file is a sample for attempte of struct data transportion" > sample.txt

list:
	${CC} ${CFLAG} -o list.o -c list.c 

