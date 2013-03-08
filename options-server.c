#include <sys/socket.h>       /*  socket definitions        */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */
#include <stdio.h>            /*  printing messages         */
#include <stdlib.h>           /*  for strtol()              */
#include <string.h>           /*  for memset()              */
#include "options-client.h"

/* Permite a los programas cliente procesar las opciones usadas al ejecutar el
 * programa para obtener la direccion ip y el puerto del servidor.
 * Llama a exit(EXIT_FAILURE) si alguna de las opciones falta.
 * Este codigo esta basado en http://www.paulgriffiths.net/program/c/srcs/echoclntsrc.html
 * y http://www.cs.utah.edu/dept/old/texinfo/glibc-manual-0.02/library_22.html
 */
void parse_options(int argc, char ** argv, struct sockaddr_in *servaddr){
  char option;
  short int port;               /*  port number   */
  char *endptr;                 /*  for strtol()  */
  int args_provided = 0;    

  memset(servaddr, 0, sizeof(servaddr));
  servaddr->sin_family = AF_INET;
  servaddr->sin_addr.s_addr = htonl(INADDR_ANY);
  
  while ((option = getopt (argc, argv, "p:")) != -1){
    switch (option) {
      case 'p':
        port = strtol(optarg, &endptr, 0);
        if ( *endptr ) {
          fprintf(stderr, "Invalid port number.\n");
          exit(EXIT_FAILURE);
	      }	      
        servaddr->sin_port = htons(port);
        ++args_provided;
        break;
      default:
        exit(EXIT_FAILURE);
    }
  }
  
  if( args_provided != 1 ){
    fprintf(stderr, "You must specify port number with the next syntax:\n\n");
    fprintf(stderr, "\t./program -p port-server\n\n");
    exit(EXIT_FAILURE);
  }
}
