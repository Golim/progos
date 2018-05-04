.PHONY: build clean

build: mydaemon writer

clean:
	@rm mydaemon writer *.o

# # daemon executable
mydaemon: daemon.o 
	gcc -o mydaemon daemon.o
	
daemon.o: daemon.c config.h
	gcc -c daemon.c
	
# # writer executable
writer: writer.o
	gcc -o writer writer.o
	
writer.o: writer.c config.h
	gcc -c writer.c
