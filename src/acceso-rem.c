#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "options-client.h"
#include "helper.h"
#include "tcp-helper.h"

#define MAX_LINE 1024

int main(int argc, char ** argv){
  int                 conn_s;               /*  connection socket         */
  char                buffer[MAX_LINE];     /*  character buffer          */
  struct sockaddr_in  servaddr;             /*  socket address structure  */

  parse_options(argc, argv, &servaddr);  

  conn_s = tcp_connect(&servaddr);
  
  /*  Read output of server, ask input to user and write input to server */
  Readline(conn_s, buffer, MAX_LINE-1);
  printf("%s", buffer);
  fgets(buffer, MAX_LINE, stdin);
  Writeline(conn_s, buffer, strlen(buffer));
  
  /* Repeat for password */  
  Readline(conn_s, buffer, MAX_LINE-1);
  printf("%s", buffer);
  fgets(buffer, MAX_LINE, stdin);
  Writeline(conn_s, buffer, strlen(buffer));
  
  /* Read authentication response */
  Readline(conn_s, buffer, MAX_LINE-1);
  printf("%s\n", buffer);

  return EXIT_SUCCESS;
}
