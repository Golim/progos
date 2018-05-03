.PHONY: build clean

build: firstdaemon writer

clean:
	@rm firstdaemon writer *.o

CC=gcc

firstdaemon: daemon.o
	$(CC) -o firstdaemon daemon.o
	
daemon.o: daemon.c
	$(CC) -c daemon.c
	
writer: writer.o
	$(CC) -o writer writer.o
	
writer.o: writer.c
	$(CC) -c writer.c
