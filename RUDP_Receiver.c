
#include "RUDP_API.c"
void print_stats(clock_t start, clock_t end, int totalReceived) {
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC; 
    double bandwidth = (totalReceived / 1024.0) / time_taken; 
    printf("Time taken: %.2f seconds\n", time_taken);
    printf("Average Bandwidth: %.2f KB/s\n", bandwidth);
}

int main() {
    int sockfd = rudp_socket();
    struct sockaddr_in serverAddr, clientAddr;
    struct RUDP_Header packetHeader;

    // Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);
    
    // Bind socket to address
    if (bind(sockfd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d\n", SERVER_PORT);
    int amount = FILESIZE / MAX_PAYLOAD_SIZE;
        if(FILESIZE % MAX_PAYLOAD_SIZE != 0)
        {
            amount++;
        }
    

    if (receiveHandshake(sockfd, &clientAddr) <= 0) {
        printf("Handshake failed.\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    clock_t start = clock();
    while (1) {
        int i = 1;
        start = clock();
        for (size_t s = 0; s < amount; s++)
        {
            while (1)
            {
                uint16_t receivedChecksum = rudp_recv(sockfd, &clientAddr, &packetHeader);
                if (receivedChecksum == i)
                {
                    sendAck(sockfd, &clientAddr);
                    i++;
                    break;
                }
            }
            
        }
        clock_t end = clock();
        printf("Received data\n");
        print_stats(start, end, FILESIZE);
        
    }

    close(sockfd);
    
    return 0;
}
