#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define MAX_PAYLOAD_SIZE 1024
#define FILESIZE 2000000
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define TIMEOUT_SEC 5
#define MAX_RETRIES 3

char *util_generate_random_data(unsigned int size) {
 char *buffer = NULL;
 // Argument check.
 if (size == 0)
 return NULL;
 buffer = (char *)calloc(size, sizeof(char));
 // Error checking.
 if (buffer == NULL)
 return NULL;
 // Randomize the seed of the random number generator.
 srand(time(NULL));
 for (unsigned int i = 0; i < size; i++)
 *(buffer + i) = ((unsigned int)rand() % 256);
 return buffer;
}

struct RUDP_Header {
    uint16_t length;  // Length of the packet
    uint16_t checksum; // Checksum for error detection
    uint8_t flags;    // Flags for various control information
    char value[MAX_PAYLOAD_SIZE];      // Data payload
};

// Function to build an RUDP packet with data
void buildRUDPPacket(struct RUDP_Header *header, char *data, uint16_t dataLength, uint16_t checksum, uint8_t flags) {
    header->length = htons(sizeof(struct RUDP_Header) + dataLength); // Total length of packet
    header->checksum = htons(checksum); // Convert checksum to network byte order
    header->flags = flags;
    // Copy data into packet after header
    memcpy(header->value, data, dataLength);
}


// Function to handle acknowledgment
int receiveAck(int sockfd, struct sockaddr_in *serverAddr) {
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

    char ackBuffer[sizeof(struct RUDP_Header)];
    socklen_t serverAddrLen = sizeof(*serverAddr);
    int numBytesReceived = recvfrom(sockfd, ackBuffer, sizeof(ackBuffer), 0, (struct sockaddr *)serverAddr, &serverAddrLen);
    if (numBytesReceived < 0) {
        printf("Acknowledge not received. Retransmitting...\n");
        return 0; // Acknowledge not received
    }
    printf("Acknowledge received.\n");
    return 1; // Acknowledge received
}

void sendAck(int sockfd, struct sockaddr_in *clientAddr) {
    struct RUDP_Header ackHeader;
    memset(&ackHeader, 0, sizeof(ackHeader));
    ackHeader.length = htons(sizeof(struct RUDP_Header));
    if (sendto(sockfd, &ackHeader, sizeof(ackHeader), 0, (const struct sockaddr *)clientAddr, sizeof(*clientAddr)) < 0) {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }
    printf("Acknowledge sent.\n");
}