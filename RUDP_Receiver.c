
#include "RUDP_API.c"


// Define the structure of the RUDP packet header


// Function to send acknowledgment


int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    struct RUDP_Header packetHeader;
    socklen_t addrLen = sizeof(clientAddr);
    //char buffer[MAX_PAYLOAD_SIZE];
    int numBytesReceived;

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
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
    
    while (1) {
        int i = 1;
        for (size_t s = 0; s < amount; s++)
        {
            while (1)
            {
            numBytesReceived = recvfrom(sockfd, &packetHeader, sizeof(packetHeader), 0, (struct sockaddr *)&clientAddr, &addrLen);
            if (numBytesReceived < 0) {
                perror("recvfrom failed");
                exit(EXIT_FAILURE);
            }
            printf("Packet received.\n");
            
            uint16_t receivedChecksum = ntohs(packetHeader.checksum);
            printf("checksum: %d\n", receivedChecksum);
            printf("i: %d\n", i);
            if (receivedChecksum == i)
            {
                sendAck(sockfd, &clientAddr);
                i++;
                break;
            }
            }
            
        }
        
        printf("Received data\n");
        
    }

    close(sockfd);
    
    return 0;
}
