#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include "options-client.h"

int main(int argc, char ** argv){
  struct sockaddr_in servaddr;

  parse_options(argc, argv, &servaddr);

}
