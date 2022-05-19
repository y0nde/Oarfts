CC = gcc
CFLAG = -ggdb

simple: 
	${CC} ${CFLAG} -o server simpleFTSserver.c
	${CC} ${CFLAG} -o client simpleFTSclient.c

struct: byteorder misc
	${CC} ${CFLAG} -o server structServer.c structCommon.c byteorder.o
	${CC} ${CFLAG} -o client structClient.c structCommon.c byteorder.o

byteorder:
	${CC} ${CFLAG} -o byteorder.o -c byteorder.c

misc:
	echo "hello world.\nThis file is a sample for attempte of struct data transportion" > sample.txt

