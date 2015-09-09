#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>
#include <fcntl.h>
#include <string.h>
 
#define BYTES 1024
void genindex();

int main(int argc, char* argv[])
{
  int one = 1, client_fd;
  int rcvd, fd, bytes_read;
  char mesg[99999], data_to_send[BYTES], path[99999];
  char c, *field[3], *ROOT;
  struct sockaddr_in svr_addr, cli_addr;
  socklen_t sin_len = sizeof(cli_addr);

  ROOT = getenv("PWD");

  while ((c = getopt (argc, argv, "r:")) != -1)
      switch (c)
      {
          case 'r':
              ROOT = malloc(strlen(optarg));
              strcpy(ROOT,optarg);
              break;
          case '?':
              fprintf(stderr,"Wrong arguments given!!!\n");
              exit(1);
          default:
              exit(1);
      }

  printf("Argument %s\n", ROOT);
 
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    err(1, "can't open socket");
 
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
 
  int port = 8080;
  svr_addr.sin_family = AF_INET;
  svr_addr.sin_addr.s_addr = INADDR_ANY;
  svr_addr.sin_port = htons(port);
 
  if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
    close(sock);
    err(1, "Can't bind");
  }

  listen(sock, 5);
  while (1) {
    client_fd = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);
    memset( (void*)mesg, (int)'\0', 99999 );
    rcvd=recv(client_fd, mesg, 99999, 0);
    printf("got connection\n");
    printf("%s\n", mesg);
 
    if (client_fd == -1) {
      perror("Can't accept");
      continue;
    }
    field[0] = strtok (mesg, " \t\n");
    printf("head %s\n",field[0]);
    field[1] = strtok (NULL, " \t");
    if ( strncmp(field[1], "/\0", 2)==0 ){
      field[1] = "/index.html";  
      genindex(ROOT);
    }
    strcpy (path, ROOT);
    strcpy(&path[strlen(ROOT)], field[1]);
    printf("newpath %s\n",path);
 
                if ( (fd=open(path, O_RDONLY))!=-1 )    //FILE FOUND
                {
                    send(client_fd, "HTTP/1.0 200 OK\n\n", 17, 0);
                    while ( (bytes_read=read(fd, data_to_send, BYTES))>0 )
                        write (client_fd, data_to_send, bytes_read);
                }
                else    write(client_fd, "HTTP/1.0 404 Not Found\n", 23); //FILE NOT FOUND

    close(client_fd);
  }
}


