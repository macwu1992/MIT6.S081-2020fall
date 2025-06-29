#include "kernel/types.h"
#include "user/user.h"

#define BUF_LEN 4
#define MAX_LEN 1024

#define STDIN   0
#define STDOUT  1

int main(int argc, char *argv[]) {
    int pipe0[2];
    int pipe1[2];

    char *ping = "ping";
    char *pong = "pong";
    char *received = "received: ";

    pipe(pipe0);
    pipe(pipe1);

    write(pipe0[1], ping, strlen(ping));
    if (fork() == 0) { // Child
        char *buf = malloc(BUF_LEN);
        char *outputStr = malloc(MAX_LEN);

        memset(buf, 0, BUF_LEN);
        memset(outputStr, 0, MAX_LEN);

        read(pipe0[0], buf, BUF_LEN);

        memcpy(outputStr+strlen(outputStr), buf, strlen(buf));
        memcpy(outputStr+strlen(outputStr), "\0\n", 2);

        printf("%d %s%s\n", getpid(), received, outputStr);

        write(pipe1[1], pong, strlen(ping));
    } else { // Parent
        char *buf = malloc(BUF_LEN);
        char *outputStr = malloc(MAX_LEN);

        memset(buf, 0, BUF_LEN);
        memset(outputStr, 0, MAX_LEN);

        read(pipe1[0], buf, BUF_LEN);

        memcpy(outputStr+strlen(outputStr), buf, strlen(buf));
        memcpy(outputStr+strlen(outputStr), "\0\n", 2);

        printf("%d %s%s\n", getpid(), received, outputStr);

        write(pipe0[1], pong, strlen(pong));
    }

    exit(0);
}