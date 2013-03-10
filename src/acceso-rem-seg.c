#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "options-client.h"
#include "secure-helper.h"

#define MAX_LINE 1024

// Very basic main: we send GET / and print the response.
int main(int argc, char **argv) {

  struct sockaddr_in servaddr;             /*  socket address structure  */
  Connection *con;
  char *output, input[MAX_LINE-1];

  parse_options(argc, argv, &servaddr);

  con = ssl_connect(&servaddr);

  /*  Read output of server, ask input to user and write input to server */
  output = ssl_read(con); // ssl_read allocate memory for output
  printf("%s", output);
  free(output);

  fgets(input, MAX_LINE, stdin);

  ssl_write(con, input);
  
  /* Repeat for password */  
  output = ssl_read(con); // ssl_read allocate memory for output
  printf("%s", output);
  free(output);

  fgets(input, MAX_LINE, stdin);

  ssl_write(con, input);

  /* Read authentication response */
  output = ssl_read(con); // ssl_read allocate memory for output
  printf("%s", output);
  free(output);

  ssl_disconnect(con);

  return EXIT_SUCCESS;
}