CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: HTTP_Client HTTP_Server

HTTP_Client: HTTP_Client.c
	$(CC) $(CFLAGS) -o HTTP_Client HTTP_Client.c

HTTP_Server: HTTP_Server.c
	$(CC) $(CFLAGS) -o HTTP_Server HTTP_Server.c -lpthread

clean:
	rm -f HTTP_Client HTTP_Server
