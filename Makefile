CC = gcc
FLAGS = -g -Wall -I include/ -lssl -lcrypto
SOURCE = main.c net.c pass.c message.c
OBJC = main.o net.o pass.o message.o
PRJC = cssl

vpath %.c src/
vpath %.h include/

$(PRJC):$(OBJC)
	$(CC) -o $@ $^ $(FLAGS)
$(OBJC):$(SOURCE)
	$(CC) -c $^ $(FLAGS)
