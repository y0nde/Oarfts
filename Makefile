CC = gcc
CFLAG = -ggdb

all: clean server client 

clean:
	rm -f server client *.o

transfer: transfer.c 
	${CC} ${CFLAG} -o transfer.o -c transfer.c

transfer-test: transfer.c byteorder connection
	${CC} ${CFLAG} -o transfer transfer.c byteorder.o connection.o

fileoperation: fileoperation.c 
	${CC} ${CFLAG} -o fileoperation.o -c fileoperation.c

test: test.c fileoperation transfer connection byteorder misc list
	${CC} ${CFLAG} -o test test.c fileoperation.o transfer.o byteorder.o connection.o list.o

byteorder:
	${CC} ${CFLAG} -o byteorder.o -c byteorder.c

connection: 
	${CC} ${CFLAG} -o connection.o -c connection.c 

filetransfer: byteorder struct connection list
	${CC} ${CFLAG} -o filetransfer.o -c filetransfer.c 

misc:
	echo "hello world.\nThis file is a sample for attempte of struct data transportion" > sample.txt

list:
	${CC} ${CFLAG} -o list.o -c list.c 
