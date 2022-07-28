UNAME_S = $(shell uname -s)

CC      = clang
CFLAGS  = -std=c11 -O0 -g
CFLAGS += -I/media/terabyte/lib/glad/include -I/media/terabyte/lib/stb/
CFLAGS += -I/media/terabyte/lib/glfw/include -I/media/terabyte/lib/cglm/include
LDFLAGS = /media/terabyte/lib/glad/src/glad.o /media/terabyte/lib/glfw/src/libglfw3.a -lm

ifeq (($UNAME_S), Linux)
	LDFLAGS += -ldl -lpthread
endif

SRC  = $(wildcard src/**/*.c) $(wildcard src/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c)
OBJ  = $(SRC:.c=.o)
BIN  = bin

.PHONY = all clean

all: dirs game

dirs:
	mkdir -p ./$(BIN)

run: all
	$(BIN)/game

game: $(OBJ)
	$(CC) -o $(BIN)/game $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
