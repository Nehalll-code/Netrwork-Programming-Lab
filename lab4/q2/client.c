#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char domain[BUF_SIZE], buffer[BUF_SIZE];

    /* 1. Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    /* 2. Server address */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* 3. Connect */
    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    /* 4. Input domain */
    printf("Enter domain name: ");
    scanf("%s", domain);

    /* 5. Send to server */
    write(sock, domain, strlen(domain));

    /* 6. Receive response */
    int n = read(sock, buffer, BUF_SIZE - 1);
    buffer[n] = '\0';

    printf("Server response: %s", buffer);

    /* 7. Close */
    close(sock);

    return 0;
}
