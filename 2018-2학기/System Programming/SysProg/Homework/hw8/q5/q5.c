#define _GNU_SOURCE

#include <fcntl.h>
#include <unistd.h>

int main(int argc,char *argv[]){
    
    int fd;
    int i;
    fd = open(argv[argc-1],O_CREAT|O_WRONLY|O_TRUNC);
    dup2(fd,1);
    argv[argc-1] = NULL;
    char ** newargv = argv + 1;
    char *path = argv[1];
    
    execv(path,newargv);
    return 0;
}
