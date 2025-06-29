#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    // check arg count
    if (argc != 2) {
        exit(-1);
    }

    char *sleepTimeStr = argv[1];
    int curLen = strlen(sleepTimeStr);

    if (curLen <= 0) {
        exit(-1);
    }

    int newLen = 0;

    if (sleepTimeStr[curLen - 1] == '\0') { // Check if original string contains '/0'
        newLen = curLen + 1;
    } else {
        newLen = curLen + 2;
    }

    char *printStr = malloc(newLen);
    memset(printStr, 0, newLen);

    memcpy(printStr, sleepTimeStr, curLen);

    printStr[newLen - 2] = '\n';
    printStr[newLen - 1] = '\0';

    write(1, printStr, strlen(printStr));

    int sleepTime = atoi(sleepTimeStr);
    sleep(sleepTime);

    exit(0);
}