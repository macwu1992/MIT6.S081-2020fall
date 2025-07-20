#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

// STRING DEFINITION
#define ERR_MSG_INVALID_ARG_COUNT      "The count of arguments must be 2!\n"
#define ERR_MSG_FAILED_TO_OPEN_FILE    "Failed to open file: "

// ARG DEFINITION
#define FIND_PATH_ARG_IDX                1   // FIND_PATH
#define FIND_TARGET_ARG_IDX              2   // FIND_TARGET

// STD IO DEFINITION
#define STD_IN                          0
#define STD_OUT                         1
#define STD_ERR                         2

// Boolean
#define TRUE                            1
#define FALSE                           0

// Other defs
#define MAX_PATH_LENGTH                 1024

// Function defs
int isSeqSubStr(char* sub, char* str);
void find(char* findTarget, char* findPath);
char* relativePath(char *path);

int main(int argc, char* args[]) {
    if (argc != 3) {
        write(1, ERR_MSG_INVALID_ARG_COUNT, strlen(ERR_MSG_INVALID_ARG_COUNT));
        exit(-1);
    }

    char* findPath = args[FIND_PATH_ARG_IDX];
    char* findTarget = args[FIND_TARGET_ARG_IDX];

    find(findTarget, findPath);

    exit(0);
}

void find(char* findTarget, char* path) {
    char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if (TRUE == isSeqSubStr(findTarget, path)) {
        printf("%s\n", path);
    }

    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }

      if(0 == strcmp(".", de.name) || 0 == strcmp("..", de.name)) {
        continue;
      }

      find(findTarget, buf);
    }
    break;
  }
  close(fd);
}

int isSeqSubStr(char* sub, char* str) {
    int subStrLength = strlen(sub);
    int strLength = strlen(str);
    int maxOffset = strLength - subStrLength;
    int result = FALSE;

    for(int offSet = 0; offSet <= maxOffset; offSet++) {
        if (0 == strcmp(sub, str + offSet)) {
            // Found seq sub string
            result = TRUE;
            break;
        }
    }

    return result;
}

char* relativePath(char *path) {
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
    return p;
    memmove(buf, p, strlen(p));
    memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}