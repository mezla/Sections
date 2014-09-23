/* A simple server TCP echo with port number is passed as an argument */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}


#define MAXREQ  256
#define MAXQUEUE 5

void server(int consockfd) {
  char reqbuf[MAXREQ];
  int n;
  while (1) {			/* echo stream on this connection */
    memset(reqbuf,0, MAXREQ);
    n = read(consockfd,reqbuf,MAXREQ-1); /* Read request from the socket */
    if (n <= 0) return;
    n = write(STDOUT_FILENO, reqbuf, strlen(reqbuf)); /* print it */
    n = write(consockfd, reqbuf, strlen(reqbuf)); /* Reply to client */
    if (n < 0) error("ERROR writing to socket");
  }
}

int startserver(int portno)
{
  int lstnsockfd, consockfd;
  struct sockaddr_in serv_addr;
  struct sockaddr_in cli_addr;
  uint clilen = sizeof(cli_addr);
  pid_t cpid;
  int socketOption = 1;
  pid_t tcpid;
  int cstatus;

  memset((char *) &serv_addr,0, sizeof(serv_addr));
  serv_addr.sin_family      = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port        = htons(portno);

  /* Create Socket to receive requests*/
  lstnsockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (lstnsockfd < 0) error("ERROR opening socket");
  printf("Listen socket: %d\n", lstnsockfd);

  if (setsockopt(lstnsockfd, SOL_SOCKET, SO_REUSEADDR, &socketOption, sizeof(socketOption)))
    error("ERROR setting reuseadd option");

  /* Bind socket to port */
  if (bind(lstnsockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
    error("ERROR on bind");

  while (1) {
    if (listen(lstnsockfd, MAXQUEUE))    /* Listen for incoming connections */
      error("ERROR on listen");

    /* Accept incoming connection, obtaining a new socket for it */
    consockfd = accept(lstnsockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (consockfd < 0) error("ERROR on accept");
    printf("connection socket: %d\n", consockfd);

    cpid = fork();		/* create new process for connection */
    if (cpid > 0) {			/* parent process */
      close(consockfd); 
      printf("Parent: of %d waitin\n", cpid);
      tcpid = wait(&cstatus);
      printf("Parent resume\n");
    } else if (cpid == 0) {		/* child process */
      close(lstnsockfd);	/* let go of listen socket */
      server(consockfd);
      printf("Closing connection\n");
      close(consockfd); 
      exit(EXIT_SUCCESS);	  /* exit child normally */
    } else {			/* fork failed */
      printf("Fork of child process failed\n");
    }
  }
  printf("Server exiting\n");
  close(lstnsockfd);
  return 0; 
}


int main(int argc, char *argv[])
{
  int portno;
  if (argc < 2) {
    fprintf(stderr,"usage %s portno\n", argv[0]);
    exit(1);
  }
  portno = atoi(argv[1]);
  printf("Opening server on port %d\n",portno);
  return startserver(portno);
}
