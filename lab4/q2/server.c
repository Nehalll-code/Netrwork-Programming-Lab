#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFSZE 1024

int main(){
    int server_fd,client_fd;
    struct sockaddr_in server_addr;
    char domain[BUFSIZ], response[BUFSIZ];
    FILE *fp;

    server_fd = socket(AF_INET,SOCK_STREAM,0);

    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));

    listen(server_fd,1);
    printf("DNS server: ");
    client_fd = accept(server_fd,NULL,NULL);

    int n = read(client_fd,domain,BUFSIZ-1);
    domain[n]='\0';
    printf("Cleint requested:");
    fp = fopen("database.txt","r");

    int found = 0;
    char db_domain[BUFSIZ], ip[BUFSIZ];

    while (fscanf(fp, "%s %s", db_domain, ip) != EOF) {
        if (strcmp(domain, db_domain) == 0) {
            sprintf(response, "IP Address: %s\n", ip);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        strcpy(response, "Domain not found\n");
    }

    /* 7. Send response */
    write(client_fd, response, strlen(response));

    /* 8. Close sockets */
    close(client_fd);
    close(server_fd);

    return 0;
}