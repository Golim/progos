.PHONY: build clean

build: daemon writer

clean:
	@rm daemon writer *.o

# # daemon executable
daemon: daemon.o 
	gcc -o daemon daemon.o
	
daemon.o: daemon.c config.h
	gcc -c daemon.c
	
# # writer executable
writer: writer.o
	gcc -o writer writer.o
	
writer.o: writer.c config.h
	gcc -c writer.c
