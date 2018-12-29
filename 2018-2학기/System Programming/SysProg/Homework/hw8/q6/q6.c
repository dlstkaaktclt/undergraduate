#define _GNU_SOURCE

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(int argc,char *argv[])
{
    char buf[160];
    int pipefd[2];
    pid_t pid,ppid;
    printf("Input string to sent to child : ");
    fgets(buf,160,stdin);
    buf[strlen(buf) - 1] = '\0';
    if(pipe(pipefd) == -1)
    {
        printf("pipe error occured!! program exit\n");
        return 1;
    }
    ppid=getpid();
    pid=fork();
    if(pid==-1)
    {
        printf("fork error occured! program exit\n");
        return 1;
    }
    if(pid==0) // child process
    {
        FILE *fp;
        if(close(pipefd[1])==-1)
        {
            printf("close error occured! program exit\n");
            return 1;
        }
        fp = fdopen(pipefd[0],"r");
        while(1)
        {
            if(fgets(buf,160,fp) == NULL)
            {
                break;
            }
        }
            
        printf("Child pid = %d, received from parent %d : %s\n",getpid(),ppid,buf);
    }
    else // parent process
    {
        FILE *fp;
        if(close(pipefd[0])==-1)
        {
            printf("close error occured! program exit\n");
            return 1;
        }
        fp = fdopen(pipefd[1],"w");
        fputs(buf,fp);
        printf("Parent pid = %d, sending to child %d : %s\n",getpid(),pid,buf);
    }
    return 0;
}
