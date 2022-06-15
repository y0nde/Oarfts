CC = gcc
CFLAG = -ggdb 
OBJD = ./obj/

all: server test

lib: fileoperation transfer connection byteorder misc list entry server
	ar rcs oarfts.a ${OBJD}fileoperation.o ${OBJD}transfer.o ${OBJD}byteorder.o ${OBJD}connection.o ${OBJD}list.o ${OBJD}entry.o

clean:
	rm -f server client *.o

transfer: transfer.c 
	${CC} ${CFLAG} -o ${OBJD}transfer.o -c transfer.c

fileoperation: fileoperation.c 
	${CC} ${CFLAG} -o ${OBJD}fileoperation.o -c fileoperation.c

test: test.c fileoperation transfer connection byteorder misc list entry
	${CC} ${CFLAG} -o test ${OBJD}test.c ${OBJD}fileoperation.o ${OBJD}transfer.o ${OBJD}byteorder.o ${OBJD}connection.o ${OBJD}list.o ${OBJD}entry.o

server: server.c fileoperation transfer connection byteorder misc list entry
	${CC} ${CFLAG} -o server server.c ${OBJD}fileoperation.o ${OBJD}transfer.o ${OBJD}byteorder.o ${OBJD}connection.o ${OBJD}list.o ${OBJD}entry.o

byteorder:
	${CC} ${CFLAG} -o ${OBJD}byteorder.o -c byteorder.c

connection: 
	${CC} ${CFLAG} -o ${OBJD}connection.o -c connection.c 

entry: 
	${CC} ${CFLAG} -o ${OBJD}entry.o -c entry.c 

misc:
	echo "hello world.\nThis file is a sample for attempte of struct data transportion" > sample.txt

list:
	${CC} ${CFLAG} -o ${OBJD}list.o -c list.c 
