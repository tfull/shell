COMPILER = gcc
SRC = src
BIN = bin
LIB = lib
CFLAGS = -Wall -O2 -fPIC

$(BIN)/main: $(BIN)/Main.o $(BIN)/Job.o $(BIN)/String.o
	$(COMPILER) $(CFLAGS) -o $@ $^

$(BIN)/Main.o: $(SRC)/Main.c
	$(COMPILER) $(CFLAGS) -I$(LIB) -c -o $@ $^

$(BIN)/Job.o: $(SRC)/Job.c
	$(COMPILER) $(CFLAGS) -I$(LIB) -c -o $@ $^

$(BIN)/String.o: $(LIB)/String.c
	$(COMPILER) $(CFLAGS) -I$(LIB) -c -o $@ $^

.PHONY: clean
clean:
	rm $(BIN)/*.o $(BIN)/main
