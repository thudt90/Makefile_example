CC  = gcc

SRC = $(wildcard *.c)
OBJ := $(patsubst %.c,%.o,$(SRC))

INCLUDE = -I./include
BUILD_DIR   = build

OBJS := $(addprefix $(BUILD_DIR)/, $(OBJ) )

edit: $(OBJ)
	$(CC) -o edit $(OBJS)

%.o: %.c
	$(CC) -c $(INCLUDE) $^ -o $(BUILD_DIR)/$@

clean :
	rm -rf edit $(BUILD_DIR)/*
