CC = gcc
CFLAG = -ggdb

all: clean server client 

clean:
	rm -f server client *.o

server:
	${CC} ${CFLAG} -o server server.c list.c byteorder.c connection.c filetransfer.c session.c structCommon.c

client:
	${CC} ${CFLAG} -o client client.c list.c byteorder.c connection.c filetransfer.c session.c structCommon.c

clientRepl:
	${CC} ${CFLAG} -o clientRepl clientRepl.c list.c byteorder.c connection.c filetransfer.c session.c structCommon.c

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
	${CC} ${CFLAG} -o filetransfer.o -c filetransfer.c 

session: 
	${CC} ${CFLAG} -o session.o -c session.c

misc:
	echo "hello world.\nThis file is a sample for attempte of struct data transportion" > sample.txt

list:
	${CC} ${CFLAG} -o list.o -c list.c 
