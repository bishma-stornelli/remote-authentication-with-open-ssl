#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

// Very basic main: we send GET / and print the response.
int main (int argc, char **argv)
{
  struct sockaddr_in  servaddr;             /*  socket address structure  */
  connection *c;
  char *response;

  parse_options(argc, argv, &servaddr);

  c = sslConnect ();

  sslWrite (c, "GET /\r\n\r\n");
  response = sslRead (c);

  printf ("%s\n", response);

  sslDisconnect (c);
  free (response);

  return 0;
}