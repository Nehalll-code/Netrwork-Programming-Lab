#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/wait.h>

#define PORT 8080
#define BUF_SIZE 1024

/* Sort characters in descending order */
void sort_desc(char *arr, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] < arr[j]) {
                char t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
}

/* Sort digits in ascending order */
void sort_asc(char *arr, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] > arr[j]) {
                char t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    /* 1. Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(1);
    }

    /* 2. Bind */
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    /* 3. Listen */
    listen(server_fd, 1);
    printf("Server waiting on port %d...\n", PORT);

    /* 4. Accept */
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("Accept failed");
        exit(1);
    }

    /* 5. Read data */
    int n = read(client_fd, buffer, BUF_SIZE - 1);
    buffer[n] = '\0';

    /* 6. Fork */
    pid_t pid = fork();

    if (pid == 0) {
        /* ===== CHILD PROCESS ===== */
        char digits[BUF_SIZE];
        int k = 0;

        for (int i = 0; buffer[i]; i++)
            if (isdigit(buffer[i]))
                digits[k++] = buffer[i];

        digits[k] = '\0';
        sort_asc(digits, k);

        char msg[BUF_SIZE];
        sprintf(msg,
                "Child PID %d → Digits ascending: %s\n",
                getpid(), digits);

        write(client_fd, msg, strlen(msg));

        close(client_fd);
        exit(0);
    } else {
        /* ===== PARENT PROCESS ===== */
        char chars[BUF_SIZE];
        int k = 0;

        for (int i = 0; buffer[i]; i++)
            if (isalpha(buffer[i]))
                chars[k++] = buffer[i];

        chars[k] = '\0';
        sort_desc(chars, k);

        char msg[BUF_SIZE];
        sprintf(msg,
                "Parent PID %d → Characters descending: %s\n",
                getpid(), chars);

        write(client_fd, msg, strlen(msg));

        /* Wait for child, then close */
        wait(NULL);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
