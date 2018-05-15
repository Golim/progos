.PHONY: build clean
SRC = src
BUILDING = build
BIN = bin

build: stats | program

clean:
	rm -Rf  $(BIN)/
	rm -Rf $(BUILDING)/
	rm -f ./log.txt
	rm -f ./log.csv
	rm -f $(SRC)/*.gch

### STATS ###
stats: stats.o   bin
	gcc $(BUILDING)/stats.o -o $(BIN)/stats 

stats.o: $(SRC)/stats/stats.c  building
	gcc -c $(SRC)/stats/stats.c  -o $(BUILDING)/stats.o

### LOGGER ###
logger.o: $(SRC)/logger/logger.c  building
	gcc -c $(SRC)/logger/logger.c -o $(BUILDING)/logger.o

### Mio_demone
program: program.o bin mp.o logger.o
	gcc $(BUILDING)/program.o $(BUILDING)/client.o  $(BUILDING)/server.o $(BUILDING)/logger.o -o $(BIN)/program 

program.o : $(SRC)/program/program.c building
	gcc -c $(SRC)/program/program.c -o $(BUILDING)/program.o

### Message_passing
mp.o: $(SRC)/message_passing/client.c  $(SRC)/message_passing/server.c building
	gcc -c $(SRC)/message_passing/client.c -o $(BUILDING)/client.o
	gcc -c $(SRC)/message_passing/server.c -o $(BUILDING)/server.o

### MAKE DIRECTORIES ###
bin:
	mkdir $(BIN) -p

building:
	mkdir $(BUILDING) -p
