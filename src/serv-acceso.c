/*

  ECHOSERV.C
  ==========
  (c) Paul Griffiths, 1999
  Email: mail@paulgriffiths.net
  
  Simple TCP/IP echo server.

*/


#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "options-server.h"   /*  parse options for servers */
#include "helper.h"           /*  our own helper functions  */
#include "usuarios.h"         /*  manejo de usuarios  */

/*  Global constants  */

#define MAX_LINE           (1000)

/*  Definition of functions */
void validate_user(int conn_s);

int main(int argc, char **argv) {
  int       list_s;                /*  listening socket          */
  int       conn_s;                /*  connection socket         */
  struct    sockaddr_in servaddr;  /*  socket address structure  */
  char      buffer[MAX_LINE];      /*  character buffer          */

  parse_options(argc, argv, &servaddr); 

  /*  Create the listening socket  */

  if ( (list_s = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
    fprintf(stderr, "Error creating listening socket.\n");
    exit(EXIT_FAILURE);
  }

  /*  Bind our socket addresss to the listening socket, and call listen()  */
  if ( bind(list_s, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
    fprintf(stderr, "Error calling bind()\n");
    exit(EXIT_FAILURE);
  }

  if ( listen(list_s, LISTENQ) < 0 ) {
    fprintf(stderr, "Error calling listen()\n");
    exit(EXIT_FAILURE);
  }
  
  /*  Enter an infinite loop to respond to client requests and echo input  */
  setbuf(stdout, NULL); // Disable buffering
  while ( 1 ) {

    /*  Wait for a connection, then accept() it  */
    printf("Esperando cliente.\t");
    if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
        fprintf(stderr, "Error calling accept()\n");
        exit(EXIT_FAILURE);
    }
    printf("Procesando solicitud.\t");
    validate_user(conn_s);

    /*  Close the connected socket  */
    printf("Cerrando conexion.\t");
    if ( close(conn_s) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error calling close()\n");
        exit(EXIT_FAILURE);
    }
  }
}

void validate_user(int conn_s){
	char buffer[MAX_LINE];      /*  character buffer          */
	char username[MAX_LINE];
	char password[MAX_LINE];

	/* Ask username and password */
	sprintf(buffer, "Nombre de usuario: \n");
	Writeline(conn_s, buffer, strlen(buffer));
	Readline(conn_s, username, MAX_LINE-1);
	/* Ask password */
	sprintf(buffer, "Clave: \n");
	Writeline(conn_s, buffer, strlen(buffer));
	Readline(conn_s, password, MAX_LINE-1);

  printf("Esperando para retrasar ataques de fuerza bruta.\n");
  sleep(2); /* Bye bye brute force */
	
	// Comparando usuario y procesando respuesta
  int res = comparar_usuario(username, password);
	switch (res) {
		case OK:
			sprintf(buffer, "Usuario autorizado\n");
			break;
		case ERROR_ARCHIVO:
			sprintf(buffer, "Error del servidor en la busqueda de usuarios\n");
			break;
		case ERROR_NOMBRE:
			sprintf(buffer, "Acceso denegado\n");
			break;
		case ERROR_PASSWORD:
			sprintf(buffer, "Acceso denegado\n");
			break;
	}
	
	Writeline(conn_s, buffer, strlen(buffer));
}