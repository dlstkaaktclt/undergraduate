/*
 * http.c - A simple, iterative HTTP/1.0 Web server that uses the
 *     GET method to serve static content.
 */
#include "csapp.h"



void doit(int fd);
void print_requesthdrs(rio_t *rp);
void parse_uri(char *uri, char *filename);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg);

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
/*
 * main:
 *  listens for connections on the given port number, handles HTTP requests
 *  with the doit function then closes the connection
 */


  int listenfd, connfd, port, clientlen;
  struct sockaddr_in clientaddr;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  port = atoi(argv[1]);
  listenfd = Open_listenfd(port);

  while(1)
  {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    //Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
    doit(connfd);
    Close(connfd);
  }

  // listen for connections on the given port
  // if a client connects, accept the connection, handle the request
  // (call the doit function), then close the connection
  return 0;
}

//-----------------------------------------------------------------------------
void doit(int fd)
{
/*
 * doit: reads a connection socket. if a correct HTTP request and method
 *        is sent, it processes the request via serve_static().
 *        Otherwise, it reports an error via clienterror().
 * params:
 *    - fd (int): file descriptor of connection socket.
 *
 */

  struct stat sbuf;
  char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
  char filename[MAXLINE];

  // read and store the first line of the HTTP request in the corresponding
  // variables (format: method uri and version)
  rio_t rio;

  Rio_readinitb(&rio, fd);

  Rio_readlineb(&rio, buf, MAXLINE);
  if(sscanf(buf,"%s %s %s\r\n",method,uri,version)<3)
  {
    clienterror(fd,"Not Correct HTTP REQUEST","400","BAD REQUEST","Please send correct http request");
    return;
  }

  if(strcmp(method,"GET")!=0)
  {
    clienterror(fd,"This server only serve GET request","501","NOT Implemented","Please sent GET request");
    return;
  }

  parse_uri(uri,filename);

  if(stat(filename,&sbuf)<0) //get file status
  {
    if(errno==ENOENT)   //if file doesn't exist
    {
      clienterror(fd,"Requested path doesn't exists","404","NOT FOUND","Please sent correct path");
      return;
    }
    else  //can't open file
    {
      clienterror(fd,"file open denied","404","NOT FOUND","can't open file");
      return;
    }
  }

  if(S_ISDIR(sbuf.st_mode))   //if file is directory
  {
    clienterror(fd,"path is directory","403","FORBIDDEN","user only access to file");
    return;
  }
  if(!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) //if server doesn't have file permission
  {
    clienterror(fd,"server can't access to file","403","FORBIDDEN","permission denied");
    return;
  }

  serve_static(fd,filename,sbuf.st_size);
  // be sure to call this only after you have read out all the information
  // you need from the request
  print_requesthdrs(&rio);

  // Check if the method is GET, if not return a 501 error using clienterror()

  // If the method is GET, first: call the parse_uri function
  //  - this returns a path from a given uri
  // Check if the requested path exists, if not return 404 Error
  // Check if the requested path is a directory if so return 403 error

  // If the file exists serve it to the client
  // (implement the serve_static function)

}

//-----------------------------------------------------------------------------
void serve_static(int fd, char *filename, int filesize)
{
/*
 * serve_static:
 *        builds and sends static HTTP requests to the client specified
 *        by the fd argument
 * params:
 *    - fd: file descriptor of connection socket.
 *    - filename: local path to file
 *    - filesize: size of requested file
 * return: void
 */

  int srcfd;
  char *srcp, filetype[MAXLINE], responseBuffer[MAXBUF];




  get_filetype(filename,filetype);

  // First check the file type using get_filetype, also add images
  // (jpg, gif, png) to the list of servable files

  sprintf(responseBuffer,"HTTP/1.0 200 OK\r\n");
  Rio_writen(fd,responseBuffer,strlen(responseBuffer));
  sprintf(responseBuffer,"Server: %s\r\n","2014-16757's tiny server");
  Rio_writen(fd,responseBuffer,strlen(responseBuffer));
  sprintf(responseBuffer,"Content-Length: %d\r\n",filesize);
  Rio_writen(fd,responseBuffer,strlen(responseBuffer));
  sprintf(responseBuffer,"Content-Type: %s\r\n\r\n",filetype);
  Rio_writen(fd,responseBuffer,strlen(responseBuffer));

  /* Send the HTTP response header to the client */
  // Build valid response header and send to client
  // (check the clienterror function for reference)
  // Should consist of a Response line, the server name, the Content-Type
  // and the Content-Length

  srcfd = Open(filename, O_RDONLY, 0);
  srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
  Rio_writen(fd, srcp, filesize);
  Close(srcfd);
  Munmap(srcp, filesize);
  /* Send the file content to the client */
  // Open the file
  // Copy it to the srcp buffer
  // Write this to the client

}

//-----------------------------------------------------------------------------
void get_filetype(char *filename, char *filetype)
{
/*
 * get_filetype:
 *        given a filename puts the correct HTTP filetype in the filetype variable
 *        by the fd argument
 * params:
 *    - filename: the filename as a path (generated by parse_uri)
 *    - filetype: the MIME type that will be included in the response to the client
 */
  if (strstr(filename, ".html"))
  {
    strcpy(filetype, "text/html");
  }
  else if(strstr(filename, ".jpg"))
  {
    strcpy(filetype, "image/jpeg");
  }
  else if(strstr(filename, ".png"))
  {
    strcpy(filetype, "image/png");
  }
  else if(strstr(filename, ".gif"))
  {
    strcpy(filetype, "image/gif");
  }
  // Please implement the image filetypes: jpg, png, gif
  else
    strcpy(filetype, "text/plain");
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

//-----------------------------------------------------------------------------
void parse_uri(char *uri, char *filename)
{
/**** DO NOT MODIFY ****/
/*
 * parse_uri:
 *        parses uri to give a local filepath, stores this in filename
 * params:
 *    - uri: uri string (input)
 *    - filename: local filepath (output)
 *
 */
  strcpy(filename, ".");
  strcat(filename, uri);
  sscanf(uri, "http://%*[^/]/%s", filename);
}

//-----------------------------------------------------------------------------
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
