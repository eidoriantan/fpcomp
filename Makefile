CC=gcc
CFLAGS=-Iinclude -static
OBJ_DIR=build
SRC_DIR=src

_OBJ=main.o compare.o read.o
OBJ=$(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

fpcomp: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o fpcomp*
