// rudp_api.h

#include <stdio.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define MAXSIZE 2097153

// Initialize RUDP connection
int rudp_init(int port);

// Send data over RUDP connection
void rudp_send(int sockfd, const char* data, struct sockaddr_in dest_addr);

// Receive data over RUDP connection
int rudp_recv(int sockfd, struct sockaddr_in* client_addr);

// Close RUDP connection
void rudp_close(int sockfd);


