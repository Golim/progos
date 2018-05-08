.PHONY: build clean

build: logger

clean:
	@rm logger *.o log.txt *.csv

# # logger executable
logger: logger.o
	gcc -o logger logger.o
	
logger.o: logger.c config.h
	gcc -c logger.c
