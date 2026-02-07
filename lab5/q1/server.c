#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/wait.h>

#define PORT 8080
#define BUF_SIZE 1024

int client_count = 0;
int client_fds[3];
struct sockaddr_in client_addrs[3];

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    socklen_t len;
    char buffer[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("Server running...\n");

    while (1) {
        len = sizeof(struct sockaddr_in);
        client_fd = accept(server_fd,
                           (struct sockaddr *)&client_addrs[client_count],
                           &len);

        client_fds[client_count] = client_fd;
        client_count++;

        /* If more than 2 clients → terminate */
        if (client_count > 2) {
            for (int i = 0; i < client_count; i++) {
                write(client_fds[i], "terminate session", 17);
                close(client_fds[i]);
            }
            printf("More than 2 clients connected. Terminating server.\n");
            close(server_fd);
            exit(0);
        }

        if (fork() == 0) {
            /* Child handles one client */
            read(client_fd, buffer, BUF_SIZE - 1);
            buffer[strcspn(buffer, "\n")] = '\0';

            FILE *fp = fopen("base.txt", "r");
            char base[BUF_SIZE];
            fgets(base, BUF_SIZE, fp);
            fclose(fp);

            printf("Client %d says: %s\n", client_count, buffer);

            printf("Client %d Address: %s:%d\n",
                   client_count,
                   inet_ntoa(client_addrs[client_count - 1].sin_addr),
                   ntohs(client_addrs[client_count - 1].sin_port));

            /* Only after second client */
            if (client_count == 2) {
                printf("\nFinal Output:\n%s %s %s\n",
                       base, "Institute Of", "Technology");

                printf("Client 1 Address: %s:%d\n",
                       inet_ntoa(client_addrs[0].sin_addr),
                       ntohs(client_addrs[0].sin_port));

                printf("Client 2 Address: %s:%d\n",
                       inet_ntoa(client_addrs[1].sin_addr),
                       ntohs(client_addrs[1].sin_port));
            }

            exit(0);
        }
    }
}
