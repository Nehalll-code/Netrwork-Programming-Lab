#include <stdio.h>
#include <stdlib.h>
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
    if (sock < 0) {
        perror("Socket failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connect failed");
        exit(1);
    }

    printf("Client connected\n");
    printf("Client Parent PID: %d, PPID: %d\n", getpid(), getppid());

    pid_t pid = fork();

    if (pid == 0) {
        /* Child process → Receive */
        printf("Client Child PID: %d, PPID: %d\n", getpid(), getppid());

        while (1) {
            int n = read(sock, buffer, BUF_SIZE);
            if (n <= 0) break;
            buffer[n] = '\0';
            printf("Server: %s", buffer);
        }
    } else {
        /* Parent process → Send */
        while (1) {
            fgets(buffer, BUF_SIZE, stdin);
            write(sock, buffer, strlen(buffer));
        }
    }

    close(sock);
    return 0;
}
