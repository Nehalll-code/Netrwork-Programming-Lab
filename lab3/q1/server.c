#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(server_fd, 1);
    printf("Server listening on port %d\n", PORT);

    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("Accept failed");
        exit(1);
    }

    printf("Server connected\n");
    printf("Server Parent PID: %d, PPID: %d\n", getpid(), getppid());

    pid_t pid = fork();

    if (pid == 0) {
        /* Child process → Receive */
        printf("Server Child PID: %d, PPID: %d\n", getpid(), getppid());

        while (1) {
            int n = read(client_fd, buffer, BUF_SIZE);
            if (n <= 0) break;
            buffer[n] = '\0';
            printf("Client: %s", buffer);
        }
    } else {
        /* Parent process → Send */
        while (1) {
            fgets(buffer, BUF_SIZE, stdin);
            write(client_fd, buffer, strlen(buffer));
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
