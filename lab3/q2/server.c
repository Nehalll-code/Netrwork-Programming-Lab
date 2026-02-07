#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF_SIZE 100

void swap(char *x, char *y) {
    char temp = *x;
    *x = *y;
    *y = temp;
}

void permute(char *str, int l, int r) {
    int i;
    if (l == r) {
        printf("%s\n", str);
        return;
    }
    for (i = l; i <= r; i++) {
        swap(&str[l], &str[i]);
        permute(str, l + 1, r);
        swap(&str[l], &str[i]); // backtrack
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    char buffer[BUF_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Server waiting for string...\n");

    len = sizeof(client_addr);
    int n = recvfrom(sockfd, buffer, BUF_SIZE, 0,(struct sockaddr *)&client_addr, &len);

    buffer[n] = '\0';


    printf("Received string: %s\n", buffer);
    printf("Permutations:\n");

    permute(buffer, 0, strlen(buffer) - 1);

    char msg[] = "Permutations printed on server";
    sendto(sockfd, msg, strlen(msg), 0,
           (struct sockaddr *)&client_addr, len);

    close(sockfd);
    return 0;
}
