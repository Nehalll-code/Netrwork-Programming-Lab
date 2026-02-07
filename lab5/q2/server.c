#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define BUF_SIZE 1024

void sort(char *s) {
    int n = strlen(s);
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (s[i] > s[j]) {
                char t = s[i];
                s[i] = s[j];
                s[j] = t;
            }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    char buffer[BUF_SIZE], response[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("Iterative Server running...\n");

    while (1) {
        len = sizeof(client_addr);
        client_fd = accept(server_fd,
                           (struct sockaddr *)&client_addr,
                           &len);

        /* Display date & time + client address */
        time_t now = time(NULL);
        printf("\nConnected to client %s:%d at %s",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port),
               ctime(&now));

        int n = read(client_fd, buffer, BUF_SIZE - 1);
        buffer[n] = '\0';

        char s1[BUF_SIZE], s2[BUF_SIZE];
        sscanf(buffer, "%[^|]|%[^\n]", s1, s2);

        if (strlen(s1) != strlen(s2)) {
            strcpy(response, "Strings are not anagrams\n");
        } else {
            sort(s1);
            sort(s2);
            if (strcmp(s1, s2) == 0)
                strcpy(response, "Strings are anagrams\n");
            else
                strcpy(response, "Strings are not anagrams\n");
        }

        write(client_fd, response, strlen(response));
        close(client_fd);   // IMPORTANT: iterative behavior
    }

    close(server_fd);
    return 0;
}
