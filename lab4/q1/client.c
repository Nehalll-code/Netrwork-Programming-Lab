#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    int option;
    char value[BUF_SIZE];

    printf("1. Registration Number\n");
    printf("2. Name of Student\n");
    printf("3. Subject Code\n");
    printf("Enter option: ");
    scanf("%d", &option);

    printf("Enter value: ");
    scanf(" %[^\n]", value);

    sprintf(buffer, "%d %s", option, value);
    write(sock, buffer, strlen(buffer));

    int n = read(sock, buffer, BUF_SIZE - 1);
    buffer[n] = '\0';

    printf("\n--- Server Response ---\n%s\n", buffer);

    close(sock);
    return 0;
}
