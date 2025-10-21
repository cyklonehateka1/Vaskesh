CC = gcc
CFLAGS = -Wall -g -Iinclude
SRC = src/main.c src/jobs.c src/parser.c src/executor.c src/ai_requests.c
OUT = myshell
READLINE = -lreadline
LIBCURL = -lcurl

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) ${READLINE} ${LIBCURL}

clean:
	rm -f $(OUT)
