#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>

#define RIO_BUFSIZE 4096

// my hand-maded rio functions..

typedef struct {
    int rio_fd;
    int rio_cnt;
    char *rio_bufptr;
    char rio_buf[RIO_BUFSIZE];
} rio_t;

void rio_init(rio_t *rp,int fd)
{
    rp->rio_fd=fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

ssize_t rio_read(rio_t *rp, void *buf, size_t count)
{
    int cnt;

    while (rp->rio_cnt<=0)
    {
        rp->rio_cnt = read(rp->rio_fd,rp->rio_buf,sizeof(rp->rio_buf));
        if(rp->rio_cnt < 0)
        {
            if(errno != EINTR)
            {
                return -1;
            }
        }
        else if(rp->rio_cnt == 0)   //EOF reached
            return 0;
        else
        rp->rio_bufptr = rp->rio_buf;
    }

    cnt = count;
    if(rp->rio_cnt < count)
    {
        cnt = rp->rio_cnt;
    }
    memcpy(buf,rp->rio_bufptr,cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

ssize_t rio_readnb(rio_t *rp,void *usrbuf,size_t n)
{
    char *buf = usrbuf;
    size_t remainsize = n;
    ssize_t readedsize = 0;

    while(remainsize>0)
    {
        readedsize = rio_read(rp,buf,remainsize);
        if(readedsize < -1)
        {
            return -1;
        }
        else if(readedsize == 0)
        {
            break;
        }
        else
        {
            remainsize -= readedsize;
            buf += readedsize;
        }
    }
    
    return (n - remainsize);
}

ssize_t rio_readlineb(rio_t *rp, void *usrbuf,size_t maxlen)
{
    char *bufp = usrbuf;
    char c;
    int cnt = 0;
    int readed = 0;
    while (cnt<maxlen-1)
    {
        readed = rio_read(rp,&c,1);
        if(readed<0) return -1;
        else if(readed == 0)
        {
            if (cnt==0) return 0;
            else break;
        }
        cnt++;
        *bufp=c;
        bufp++;
        if(c=='\n')
        {
            cnt++;
            break;
        }
    }
    *bufp='\0';
    return cnt;
}

ssize_t rio_readn(int fd, void *buf, size_t count)
{
    char *bufp=buf;
    size_t remain=count;
    ssize_t readed;

    while(remain>0)
    {
         readed = read(fd,bufp,remain);
         if(readed < 0)
         {
            if(errno != EINTR)
            {
                return -1;
            }
            else
            {
                readed = 0;
            }
         }
         else if(readed == 0)
         {
            break;
         }
         remain -= readed;
         bufp += readed;
    }

    return (count - remain);         
}

ssize_t rio_writen(int fd, const void *buf, size_t count)
{
    const char *bufp = buf;
    size_t remain = count;
    ssize_t writed = 0;

    while(remain>0)
    {
        writed = write(fd,bufp,remain);
        if(writed < 0)
        {
            if(errno != EINTR)
            {
                return -1;
            }
            else writed=0;
        }
        else
        {
            bufp += writed;
            remain -= writed;
        }
    }

    return (count - remain);
}

int open_clientfd(char *hostname, char *port)
{
    int clientfd;
    struct addrinfo hint;
    struct addrinfo *p,*listp;
    int temp;
    
    memset(&hint,0,sizeof(struct addrinfo));
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_protocol = 0;
    hint.ai_flags = AI_NUMERICSERV;
    hint.ai_flags |= AI_ADDRCONFIG;

    temp = getaddrinfo(hostname,port,&hint,&listp);
    if(temp!=0)
    {
        printf("%s error occured\n",gai_strerror(temp));
        fflush(stdout);
        return -1;
    }
    for(p = listp;p;p=p->ai_next)
    {
        if((clientfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) < 0)
        {
            continue;
        }
        else
        
        if(connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
        {
            break;
        }
        close(clientfd);
    }
    freeaddrinfo(listp);
    if(p!=NULL) return clientfd;
    else return -1;
}

int main(int argc,char *argv[])
{
    int clientfd;
    rio_t rio;
    char buf[1024];

    if(argc != 3)
    {
        printf("correct arguments, please\n");
        return 1;
    }

    clientfd = open_clientfd(argv[1],argv[2]);
    rio_init(&rio,clientfd);
    
    while (1)
    {
        printf("type: ");
        fflush(stdout);
        if(fgets(buf,1024,stdin)==NULL || buf[0]=='\n')
        {
            break;
        }
        rio_writen(clientfd,buf,strlen(buf));
        rio_readlineb(&rio,buf,1024);
        fputs("echo: ",stdout);
        fputs(buf,stdout);
    }
    close(clientfd);
    return 0;
}

