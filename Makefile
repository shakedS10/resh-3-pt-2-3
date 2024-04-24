CC = gcc
CFLAGS = -Wall -g

all: RUDP_Receiver RUDP_Sender

RUDP_Receiver: RUDP_Receiver.o 
	$(CC) $(CFLAGS) -o RUDP_Receiver RUDP_Receiver.o

RUDP_Sender: RUDP_Sender.o 
	$(CC) $(CFLAGS) -o RUDP_Sender RUDP_Sender.o

RUDP_Receiver.o: RUDP_Receiver.c RUDP_API.c
	$(CC) $(CFLAGS) -c RUDP_Receiver.c

RUDP_Sender.o: RUDP_Sender.c RUDP_API.c
	$(CC) $(CFLAGS) -c RUDP_Sender.c

.PHONY: clean all

clean:
	rm -f *.o RUDP_Receiver RUDP_Sender