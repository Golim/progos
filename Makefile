.PHONY: build clean
SRC = src
BUILDING = obj
BIN = bin

CC = gcc -std=gnu90 -g

build: main

clean:
	rm -Rf  $(BIN)/
	rm -Rf $(BUILDING)/
	rm -f ./log.txt
	rm -f ./log.csv
	rm -f $(SRC)/*.gch

### MAIN ###
main: main.o messanger.o execute_commands.o program.o logger.o stats.o  parser.o util.o sys_wrapper.o  bin 
	$(CC) $(BUILDING)/main.o $(BUILDING)/util.o $(BUILDING)/execute_commands.o $(BUILDING)/program.o $(BUILDING)/client.o $(BUILDING)/server.o $(BUILDING)/logger.o $(BUILDING)/sys_wrapper.o  $(BUILDING)/parser.o $(BUILDING)/stats.o  -o $(BIN)/stats

main.o: building
	$(CC) -c $(SRC)/main.c  -o $(BUILDING)/main.o

### STATS ###
stats.o: $(SRC)/stats/stats.c  building
	$(CC) -c $(SRC)/stats/stats.c  -o $(BUILDING)/stats.o

### PARSER ###
parser.o: $(SRC)/parser/parser.c  building
	$(CC) -c $(SRC)/parser/parser.c -o $(BUILDING)/parser.o

### Execute Commands ###
execute_commands.o: $(SRC)/execute/execute_commands.c building
	$(CC) -c $(SRC)/execute/execute_commands.c -o $(BUILDING)/execute_commands.o

### Program ###
program.o : $(SRC)/program/program.c building
	$(CC) -c $(SRC)/program/program.c -o $(BUILDING)/program.o

### messanger
messanger.o: $(SRC)/messanger/client.c  $(SRC)/messanger/server.c building
	$(CC) -c $(SRC)/messanger/client.c -o $(BUILDING)/client.o
	$(CC) -c $(SRC)/messanger/server.c -o $(BUILDING)/server.o

### LOGGER ###
logger.o: $(SRC)/logger/logger.c  building
	$(CC) -c $(SRC)/logger/logger.c -o $(BUILDING)/logger.o

### UTIL ###
util.o: $(SRC)/util/util.c  building
	$(CC) -c $(SRC)/util/util.c -o $(BUILDING)/util.o

### SYS_WRAPPER ###
sys_wrapper.o: $(SRC)/sys_wrapper/sys_wrapper.c  building
	$(CC) -c $(SRC)/sys_wrapper/sys_wrapper.c -o $(BUILDING)/sys_wrapper.o
### MAKE DIRECTORIES ###
bin:
	mkdir $(BIN) -p

building:
	mkdir $(BUILDING) -p
