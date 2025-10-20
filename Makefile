CC = gcc
CFLAGS = -Wall -g -Iinclude
SRC = src/main.c src/jobs.c src/parser.c src/executor.c
OUT = myshell

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)
