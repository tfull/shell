SRC = src
BIN = bin
LIB = library/C

$(BIN)/main: $(BIN)/Main.o $(BIN)/Job.o $(BIN)/String.o
	g++ -Wall -o $@ $^

$(BIN)/Main.o: $(SRC)/Main.c
	g++ -Wall -I$(LIB) -c -o $@ $^

$(BIN)/Job.o: $(SRC)/Job.c
	g++ -Wall -I$(LIB) -c -o $@ $^

$(BIN)/String.o: $(LIB)/String.c
	g++ -Wall -I$(LIB) -c -o $@ $^
