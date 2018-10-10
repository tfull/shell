SRC = src
BIN = bin
LIB = lib
CFLAGS = -Wall -O2

$(BIN)/main: $(BIN)/Main.o $(BIN)/Job.o $(BIN)/String.o
	g++ $(CFLAGS) -o $@ $^

$(BIN)/Main.o: $(SRC)/Main.c
	g++ $(CFLAGS) -I$(LIB) -c -o $@ $^

$(BIN)/Job.o: $(SRC)/Job.c
	g++ $(CFLAGS) -I$(LIB) -c -o $@ $^

$(BIN)/String.o: $(LIB)/String.c
	g++ $(CFLAGS) -I$(LIB) -c -o $@ $^

.PHONY: clean
clean:
	rm $(BIN)/*.o $(BIN)/main
