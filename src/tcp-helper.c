#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tcp-helper.h"

// Establish a regular tcp connection
int tcp_connect(struct sockaddr_in *servaddr) {
  int handle;
  struct hostent *host;

  /*  Create the listening socket  */

  if ( (handle = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    fprintf(stderr, "Error creating listening socket.\n");
    exit(EXIT_FAILURE);
  }
  
  /*  connect() to the remote echo server  */

  if ( connect(handle, (struct sockaddr *) &servaddr, sizeof(servaddr) ) < 0 ) {
    fprintf(stderr, "Error calling connect()\n");
    exit(EXIT_FAILURE);
  }

  return handle;
}