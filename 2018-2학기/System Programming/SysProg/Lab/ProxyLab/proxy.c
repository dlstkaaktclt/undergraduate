/*
 * proxy.c - A simple, iterative HTTP proxy server
 *
 * it implements concurrency proxy server use thread.
 *
 *
 * main routine
 *
 * 1. initialize cache, semaphore, and signal handler.
 * 2. make listenfd, and wait for connection.
 * 3. if connection maked, then make new thread and thread serve client.
 * 4. go to 2.
 *
 * thread routine
 *
 * 1. get client's fd
 * 2. get request line & header
 * 3. parse information, and if request is cached,
 *    get content from cache, send to client, and goto 9.
 * 4. parse information, and make connection to server
 * 5. send server to request, and get response by server
 * 6. send response to client.
 * 7. make new cache block at cache.
 * 8. disconnect server
 * 9. disconnect client.
 */


#include "csapp.h"
#include "cache.h"

void doit(int fd);
void print_requesthdrs(rio_t *rp);
int parse_uri_proxy(char* uri, char* host, int *port,char* filename);
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg);


void *thread_routine(void *arg);
void sigpipe_handler(int signal);
//sigpipe handler to handle Sigpipe signal well.

ssize_t my_Rio_readn(int fd, void *usrbuf, size_t n);
void my_Rio_writen(int fd, void *usrbuf, size_t n);
ssize_t my_Rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t my_Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
//custom my_rio_funtions to handle errors.
//these functions prevents exiting whole process when error occurs.
//just kill current thread.


static sem_t cache_write_mutex;  // semaphores to handle cache.
static sem_t cache_read_mutex;
//static sem_t maxthread;

static int readcnt;     // to store number of readers.





//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
/*
 * main:
 *  listens for connections on the given port number, handles HTTP requests
 *  with the doit function then closes the connection
 */
  int listenfd, port, clientlen;
  struct sockaddr_in clientaddr;
  pthread_t tid;
  int *connfd;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  port = atoi(argv[1]);
  listenfd = Open_listenfd(port); // get listenfd
  Sem_init(&cache_write_mutex,0,1);
  Sem_init(&cache_read_mutex,0,1);
  readcnt=0;
  Signal(SIGPIPE, sigpipe_handler);
  cache_init();
  //initialize cache, semaphore and sighandler.
  while(1)
  {
    clientlen = sizeof(clientaddr);
    connfd = (int *) Malloc(sizeof(int));  // malloc connfd to prevent race by thread and main routine
    *connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    Pthread_create(&tid,NULL,thread_routine,(void *)connfd);
    //use thread to process many requests.
  }


  // listen for connections
  // if a client connects, accept the connection, handle the requests
  // (call the do it function), then close the connection
}

// thread routine for client.
void *thread_routine(void *arg)
{
    int fd = *((int *)arg);
    Free(arg);      //get fd and freeing.
    Pthread_detach(Pthread_self());   //thread_detach to prevent zombie thread.
    doit(fd);
    Close(fd);
    return NULL;
}

// if sigpipe occured, call this handler.
// this prevent whole process killed by SIGPIPE (like reconnecting error)
void sigpipe_handler(int signal)
{
    unsigned int ttid = (unsigned int) Pthread_self();
    fprintf(stderr, "%u %s: %s\n",ttid, "Thread Killed", strerror(errno));
    Pthread_cancel(Pthread_self());
    return;
}


//-----------------------------------------------------------------------------
void doit(int fd)
{
/*
 * doit: reads HTTP request from connection socket, forwards the request to the
 *  requested host. Reads the response from the host server, and writes the
 *  response back to the client then save contents in the proxy cache.
 * params:
 *    - fd (int): file descriptor of the connection socket.
 */
  char linel[MAXLINE], host[MAXLINE], filename[MAXLINE];
  char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
  int serverfd, port;

  rio_t rio;

  Rio_readinitb(&rio, fd);
  linel[0] = '\0';
  /* Read request header */
  //Read line1
  // format: method uri version
  Rio_readlineb(&rio, linel, MAXLINE);
  if(sscanf(linel,"%s %s %s\r\n",method,uri,version)<3)
  {
    clienterror(fd,"Not Correct HTTP REQUEST","400","BAD REQUEST","Please send correct http request");
    return;
  }
  // Check if the method is GET, if not return a 501 error using clienterror()
  if(strcmp(method,"GET")!=0)
  {
    clienterror(fd,"This server only serve GET request","501","NOT Implemented","Please sent GET request");
    return;
  }
  // Get hostname, port, filename by parse_uri()
  if(parse_uri_proxy(uri,host,&port,filename)!=1)
  {
     clienterror(fd,"Not Correct HTTP REQUEST","400","BAD REQUEST","Please send correct url");
     return;
  }
  // If uri is in proxy cache, send the cached content to the client


  // use reader(cache read)-writer(cache write) model to protect cache.
  // use reader-preference model.
  P(&cache_read_mutex);
  readcnt++;
  if(readcnt==1) P(&cache_write_mutex);
  V(&cache_read_mutex);     //reader try to read cache

  cache_block *cblock = find_cache_block(uri);
  if(cblock!=NULL)      //if cacheblock Found, send contents.
  {
      my_Rio_writen(fd,cblock->response,strlen(cblock->response));
      my_Rio_writen(fd,cblock->contents,cblock->contentsize);

      P(&cache_read_mutex);
      readcnt--;
      if(readcnt==0) V(&cache_write_mutex);
      V(&cache_read_mutex);
      return;
  }
  else
  {
      P(&cache_read_mutex);
      readcnt--;
      if(readcnt==0) V(&cache_write_mutex);
      V(&cache_read_mutex);
  }

  //reader out.


  // Be sure to call this only after you have read out all the information
  // you need from the request
  print_requesthdrs(&rio);







  char responseBuffer[MAXLINE], response[MAXLINE];
  char responseState[MAX_RESP_SIZE];  //variable to save Http response state
  int isChunked,isVary;
  char *contentBuffer;
  int contentLength=0;
  rio_t serverResponse;
  int notBeCache = 0;

  responseBuffer[0] = '\0';
  response[0] = '\0';
  responseState[0] = '\0';
  isChunked = 0;
  isVary = 0;
  //initializing values

  serverfd = open_clientfd(host,port);  //get clientfd

  int refusecount=0;
  while(serverfd < 0)
  {
     if(errno == ECONNREFUSED)      //if connection refused by server, wait and reconnecting.
     {
         refusecount++;
         printf("%s\n",linel);
         printf("reconnecting..\n");
         printf("%s\n",strerror(errno));
         sleep(1);
         serverfd=open_clientfd(host,port);
         if(refusecount>10)
         {
             clienterror(fd,"Can't connect Server","503","SERVER Refuse Connect","Please retry sometimes later");
             return;
         }
     }
     else       //else send errmsg to client
     {
         clienterror(fd,"Can't connect Server","502","Proxy SERVER can't Connect to server","Please send correct url");
         return;
     }

  }

  linel[0] = '\0';
  sprintf(linel,"GET /%s HTTP/1.1\r\n",filename);

  my_Rio_writen(serverfd,linel,strlen(linel));  // connect server and send message

  sprintf(linel,"Host: %s\r\n",host);
  my_Rio_writen(serverfd,linel,strlen(linel));  // send host name

  linel[0] = '\0';
  strcat(linel,"\r\n");
  my_Rio_writen(serverfd,linel,strlen(linel));  // send all http request.



  Rio_readinitb(&serverResponse, serverfd);

  my_Rio_readlineb(&serverResponse,responseBuffer,MAXLINE);
  strcat(response,responseBuffer);
  strcat(responseState,responseBuffer);
  // read response status line
  while(1) // get response header. if get \r\n, terminate loop
  {
     my_Rio_readlineb(&serverResponse,responseBuffer,MAXLINE);
     if(strcmp(responseBuffer,"\r\n")==0) break;
     strcat(response,responseBuffer);
     if(strstr(responseBuffer,"Content-Length"))    //if content-length detected, get content-length
     {
         isVary = 1;
         if(sscanf(responseBuffer,"%*[^0123456789]%d",&contentLength)<1) //if can't read contentLength, terminate.
         {
             clienterror(fd,"Server doesn't send correct header","400","BAD REQUEST","server problem");
             Pthread_cancel(Pthread_self());
         }
     }
     if(strstr(responseBuffer,"Transfer-Encoding"))  // if get chunked message, get data by chunked
     {
         if(strstr(responseBuffer,"chunked"))
         {
             isChunked = 1;
         }
     }
  }
  strcat(response,responseBuffer);
  my_Rio_writen(fd,response,strlen(response)); // send response header to client



  /* Read response header*/
  // Read the response header from the server and build the proxy's response
  // header by repeatedly adding the responseBuffer (server response)
  // You need to check whether 'Content-Length' or 'Transfer Encoding: chunked'
  //
  // 'Content-Length', we must parse the Content-Length header from the server response
  // in order to know how much content to read from the server and write to the client
  //


  if(isChunked)
  {
      int chunkLength=0;
      int accumLength=0;            //to cache object, we need calculate total size of content.
      char *tempBuffer;
      char *nowContentBuffer = (char *)Mmap(NULL,MAX_CONTENT_SIZE,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS , -1, 0);
      contentBuffer = nowContentBuffer;
      //get new space by mmap. contentbuffer is used as cached content.

      my_Rio_readlineb(&serverResponse,responseBuffer,MAXLINE);  //read chunkheader
      sscanf(responseBuffer,"%x",&chunkLength); //get contentLength
      while(chunkLength!=0)   //while end of chunked data,
      {
          my_Rio_writen(fd,responseBuffer,strlen(responseBuffer)); //send chunkheader to client

          tempBuffer = (char *) Malloc(chunkLength);
          my_Rio_readnb(&serverResponse,tempBuffer,chunkLength);       //get chunk content
          my_Rio_writen(fd,tempBuffer,chunkLength);            //send chunk content
          accumLength += chunkLength;
          if(notBeCache) //if content size is bigger than max cacheblock size, just ignore.
          {
              notBeCache = 1;
          }
          else if(accumLength>MAX_CONTENT_SIZE) //if content size is bigger than max cacheblock size, unmap contentbuffer.
          {
              notBeCache = 1;
              Munmap(contentBuffer,MAX_CONTENT_SIZE);
          }
          else  // copy content to contentbuffer.
          {
              memcpy(nowContentBuffer, tempBuffer, chunkLength);
              nowContentBuffer += chunkLength;
          }
          Free(tempBuffer);
          my_Rio_readlineb(&serverResponse,responseBuffer,MAXLINE); //endof content. read \r\n
          my_Rio_writen(fd,responseBuffer,strlen(responseBuffer)); //send chunk tail
          my_Rio_readlineb(&serverResponse,responseBuffer,MAXLINE);  //read chunkheader
          sscanf(responseBuffer,"%x",&chunkLength); //get contentLength
      }
      my_Rio_writen(fd,responseBuffer,strlen(responseBuffer)); // end connection.

      if(!notBeCache) // if content should cached, unmap current contentbuffer and make new contentbuffer to store.
      {
          contentLength = accumLength;
          nowContentBuffer = contentBuffer;
          contentBuffer = (char *)Mmap(NULL,contentLength,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS , -1, 0);
          memcpy(contentBuffer,nowContentBuffer,contentLength);
          Munmap(nowContentBuffer,MAX_CONTENT_SIZE);
      }
  }
  /* Transfer Encoding: chunked */
  // Using the 'Transfer Encoding' read from the http server response header,
  // you must parse encoded chunked content. A process for decoding the chunked content,
  // You can refer pseudo-code in RFC2616 section 19.4.6
  else if(isVary)
  {
        contentBuffer = (char *)Mmap(NULL,contentLength,PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS , -1, 0);
        my_Rio_readnb(&serverResponse,contentBuffer,contentLength); //get content.
        my_Rio_writen(fd,contentBuffer,contentLength); // send content to client
        if(contentLength>MAX_CONTENT_SIZE) // if content is bigger than cache block, just unmap.
        {
            notBeCache = 1;
            Munmap(contentBuffer,contentLength);
        }
  }
  else //if proxy server doesn't find correct method to get content, send errmsg to client.
  {
      Close(serverfd);
      clienterror(fd,"not implemented data transfer method","501","NOT IMPLEMENTED","proxy server only understand chunked & content-length method");
      return;
  }
  /* Content-Length */
  // Using the 'Content-Length' read from the http server response header,
  // you must allocate and read that many bytes to our buffer
  // you now write the response heading and the content back to the client

  if(!notBeCache) //caching content.
  {
      char *cacheuri = Malloc(MAX_URL_SIZE);
      char *cacheresponse = Malloc(MAX_RESP_SIZE);
      cacheresponse[0] = '\0';
      cacheuri[0] = '\0';
      char buff[512];

      strcat(cacheuri,uri);
      strcat(cacheresponse,responseState);
      strcat(cacheresponse,"Server: 2014-16757's tiny proxy server\r\n");
      buff[0] = '\0';
      sprintf(buff,"Content-Length: %d\r\n\r\n",contentLength);
      strcat(cacheresponse,buff); //make response.
      P(&cache_write_mutex);
      add_cache_block(cacheuri,cacheresponse,contentBuffer,contentLength);
      V(&cache_write_mutex); // add contents to cache.
  }

  /* Save the proxy cache */
  // Save the contents you get from the http server.
  Close(serverfd);
  // Close the connection to the server
}

//-----------------------------------------------------------------------------
void print_requesthdrs(rio_t *rp)
{
/**** DO NOT MODIFY ****/
/**** WARNING: This will read out everything remaining until a line break ****/
/*
 * print_requesthdrs:
 *        reads out and prints all request lines sent to the server
 * params:
 *    - rp: Rio pointer for reading from file
 *
 */
  char buf[MAXLINE];
  Rio_readlineb(rp, buf, MAXLINE);
  while(strcmp(buf, "\r\n")) {
    printf("%s", buf);
    Rio_readlineb(rp, buf, MAXLINE);
  }
  printf("\n");
  return;
}

int parse_uri_proxy(char* uri, char* host, int* port, char* filename)
{
/*
 * parse_uri - Get the hostname, port, filename.
 * You have to add further parsing steps in http.c
 *
 * Example1: http://www.snu.ac.kr/index.html
 *			 host: www.snu.ac.kr
 *			 filename: should be /index.html
 *			 port: 80 (default)
 *
 * Example2: http://www.snu.ac.kr:1234/index.html
 * 			 host: www.snu.ac.kr
 * 			 filename: /index.html
 * 			 port: 1234
 *
 * Example3: http://127.0.0.1:1234/index.html
 * 			 host: 127.0.0.1
 * 			 filename: /index.html
 * 			 port: 1234
 *
 *
 * Return value for parsing success is 1
 *
 */
    int success=0;
    if (sscanf(uri, "http://%200[^:]:%i/%200[^\n]", host, port, filename) == 3) success = 1;
    else if (sscanf(uri, "http://%200[^/]/%200[^\n]", host, filename) == 2) success = 2;
    else if (sscanf(uri, "http://%200[^:]:%i[^\n]", host, port) == 2) success = 3;
    else if (sscanf(uri, "http://%200[^\n]", host) == 1) success = 4;

    if(success==0)
    {
        return 0;
    }
    else if(success%2==0)
    {
        *port = 80;
    }

    if(success>2)
    {
        filename[0] = '\0';
    }
    return 1;

}

void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg)
{
/**** DO NOT MODIFY ****/
/*
 * clienterror:
 *        Creates appropriate HTML error page and sends to the client
 * params:
 *    - fd: file descriptor of connection socket.
 *    - cause: what has caused the error: the filename or the method
 *    - errnum: The HTTP status (error) code
 *    - shortmsg: The HTTP status (error) message
 *    - longmsg: A longer description of the error that will be printed
 *           on the error page
 *
 */
  char buf[MAXLINE], body[MAXBUF];

  /* Build the HTTP response body */
  sprintf(body, "<html><title>Mini Error</title>");
  sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
  sprintf(body, "%s<b>%s: %s</b>\r\n", body, errnum, shortmsg);
  sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
  sprintf(body, "%s<hr><em>Mini Web server</em>\r\n", body);

  /* Print the HTTP response */
  sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-type: text/html\r\n");
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-Length: %d\r\n\r\n", (int)strlen(body));
  Rio_writen(fd, buf, strlen(buf));
  Rio_writen(fd, body, strlen(body));
}



ssize_t my_Rio_readn(int fd, void *ptr, size_t nbytes)
{
    ssize_t n;

    if ((n = rio_readn(fd, ptr, nbytes)) < 0)
    {
        unsigned int ttid = (unsigned int) Pthread_self();
        fprintf(stderr, "%u %s: %s\n",ttid, "Thread Killed", strerror(errno));
        Pthread_cancel(Pthread_self());
    }

    return n;
}

void my_Rio_writen(int fd, void *usrbuf, size_t n)
{
    if (rio_writen(fd, usrbuf, n) != n)
    {
        unsigned int ttid = (unsigned int) Pthread_self();
        fprintf(stderr, "%u %s: %s\n",ttid, "Thread Killed", strerror(errno));
        Pthread_cancel(Pthread_self());
    }
}

ssize_t my_Rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
    ssize_t rc;

    if ((rc = rio_readnb(rp, usrbuf, n)) < 0)
	{
	    unsigned int ttid = (unsigned int) Pthread_self();
        fprintf(stderr, "%u %s: %s\n",ttid, "Thread Killed", strerror(errno));
        Pthread_cancel(Pthread_self());
	}
    return rc;
}

ssize_t my_Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    ssize_t rc;

    if ((rc = rio_readlineb(rp, usrbuf, maxlen)) < 0)
	{
	    unsigned int ttid = (unsigned int) Pthread_self();
        fprintf(stderr, "%u %s: %s\n",ttid, "Thread Killed", strerror(errno));
        Pthread_cancel(Pthread_self());
	}
    return rc;
}
