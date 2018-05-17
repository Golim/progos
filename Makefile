.PHONY: build clean
SRC = src
BUILDING = build
BIN = bin

CC = gcc -std=gnu90

build: main

clean:
	rm -Rf  $(BIN)/
	rm -Rf $(BUILDING)/
	rm -f ./log.txt
	rm -f ./log.csv
	rm -f $(SRC)/*.gch

### MAIN ###
main: main.o message_passing.o program.o logger.o stats.o bin
	$(CC) $(BUILDING)/main.o $(BUILDING)/program.o $(BUILDING)/client.o $(BUILDING)/server.o $(BUILDING)/logger.o  $(BUILDING)/stats.o -o $(BIN)/main

main.o: building
	$(CC) -c $(SRC)/main.c  -o $(BUILDING)/main.o

### STATS ###
stats: stats.o   bin
	$(CC) $(BUILDING)/stats.o -o $(BIN)/stats 

stats.o: $(SRC)/stats/stats.c  building
	$(CC) -c $(SRC)/stats/stats.c  -o $(BUILDING)/stats.o

### LOGGER ###
logger.o: $(SRC)/logger/logger.c  building
	$(CC) -c $(SRC)/logger/logger.c -o $(BUILDING)/logger.o

### Program ###
program.o : $(SRC)/program/program.c building
	$(CC) -c $(SRC)/program/program.c -o $(BUILDING)/program.o

### Message_passing
message_passing.o: $(SRC)/message_passing/client.c  $(SRC)/message_passing/server.c building
	$(CC) -c $(SRC)/message_passing/client.c -o $(BUILDING)/client.o
	$(CC) -c $(SRC)/message_passing/server.c -o $(BUILDING)/server.o

### MAKE DIRECTORIES ###
bin:
	mkdir $(BIN) -p

building:
	mkdir $(BUILDING) -p
