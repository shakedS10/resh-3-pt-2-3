#include "RUDP_API.c"

int main(int argc, char *argv[]) {
    char* ip = argv[1];
    int port = atoi(argv[2]);
    int sockfd = rudp_socket();
    struct sockaddr_in serverAddr;
    struct RUDP_Header packetHeader;
    uint16_t textLength = MAX_PAYLOAD_SIZE;
    uint8_t packetFlags = 0x01; // deaful flags

    // Configure server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &serverAddr.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    // Perform handshake with the receiver
    if (performHandshake(sockfd, &serverAddr) <= 0) {
        printf("Handshake failed. Exiting.\n");
        close(sockfd);
        return 1;
    }

    // Generate random data to send and extract it from a file to array
    char *fdata = util_generate_random_data(FILESIZE);
    FILE *file = fopen("a.txt", "w");

    if (file == NULL) {
        perror("Error opening file");
        return 1; // Return an error code
    }
    fwrite(fdata, sizeof(char), FILESIZE, file);
    fclose(file);
    FILE *fr = fopen("a.txt", "r");
    if (fr == NULL) {
        perror("Error opening file");
        return 1; // Return an error code
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("File size: %ld\n", fileSize);
    char *data = (char *)malloc(fileSize + 1);
     if (data == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return 1;
    }
    size_t bytesRead = fread(data, sizeof(char), fileSize, file);
    data[bytesRead] = '\0';
    fclose(file);
    int check = 1;
    int i = 1;
    while (check) {
        i = 1;
        int amount = fileSize / MAX_PAYLOAD_SIZE;
        if(fileSize % MAX_PAYLOAD_SIZE != 0)
        {
            amount++; //find out how many packets we need to send
        }
        while (i <= amount)
        {
            char txt[MAX_PAYLOAD_SIZE];
            for (size_t s = 0; s < MAX_PAYLOAD_SIZE; s++)
            {
                txt[s] = data[i*MAX_PAYLOAD_SIZE + s]; //split text to smaller buffers so they can fit in packets
            }
            buildRUDPPacket(&packetHeader, txt, textLength, i, packetFlags); //build packet
            rudp_send(sockfd, &serverAddr, &packetHeader); //send packet
            i++;
        }
        char act; 
        printf("Do you want to send more data? (y/n)\n");
        scanf(" %c", &act);
        if (act == 'n')
        {
            check = 0;
        }
        else if (act == 'y')
        {
            check = 1;
        }   
        
    }
    free(data);
    free(fdata);
    close(sockfd);
    
    return 0;
}
