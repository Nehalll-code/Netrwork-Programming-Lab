#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 1);

    printf("Server waiting...\n");
    client_fd = accept(server_fd, NULL, NULL);

    int n = read(client_fd, buffer, BUF_SIZE - 1);
    buffer[n] = '\0';

    int option;
    char value[BUF_SIZE];

    sscanf(buffer, "%d %[^\n]", &option, value);

    /* CHILD 1 – Registration Number */
    if (fork() == 0) {
        if (option == 1) {
            char msg[BUF_SIZE];
            sprintf(msg,
                    "PID %d\nName: Nehal\nAddress: Bangalore\n",
                    getpid());
            write(client_fd, msg, strlen(msg));
        }
        exit(0);
    }

    /* CHILD 2 – Name */
    if (fork() == 0) {
        if (option == 2) {
            char msg[BUF_SIZE];
            sprintf(msg,
                    "PID %d\nDept: CCE\nSemester: 3\nSection: B\nCourses: OS, CN, DBMS\n",
                    getpid());
            write(client_fd, msg, strlen(msg));
        }
        exit(0);
    }

    /* CHILD 3 – Subject Code */
    if (fork() == 0) {
        if (option == 3) {
            char msg[BUF_SIZE];
            sprintf(msg,
                    "PID %d\nSubject: %s\nMarks: 87\n",
                    getpid(), value);
            write(client_fd, msg, strlen(msg));
        }
        exit(0);
    }

    /* Parent waits */
    wait(NULL);
    wait(NULL);
    wait(NULL);

    close(client_fd);
    close(server_fd);
    return 0;
}
