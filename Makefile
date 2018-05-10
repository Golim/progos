.PHONY: build clean
SRC = src
BUILDING = build
BIN = bin

build: stats | logger

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

### MESSAGING ###
messaging.o: $(SRC)/messaging/messaging.c  building
	gcc -c $(SRC)/messaging/messaging.c -o $(BUILDING)/messaging.o

### LOGGER ###
logger: logger.o  messaging.o bin
	gcc $(BUILDING)/logger.o $(BUILDING)/messaging.o -o $(BIN)/logger 
	
logger.o: $(SRC)/logger/logger.c  building
	gcc -c $(SRC)/logger/logger.c -o $(BUILDING)/logger.o
### MAKE DIRECTORIES ###
bin:
	mkdir $(BIN) -p

building:
	mkdir $(BUILDING) -p
