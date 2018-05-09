.PHONY: build clean

build: stats

clean:
	@rm stats *.o 

# # create executable
stats: stats.o
	gcc -o stats stats.o

stats.o: stats.c
	gcc -c stats.c