#ifndef OPTIONS_CLIENT_H
#define OPTIONS_CLIENT_H

#include <arpa/inet.h>        /*  inet (3) funtions         */

/* Permite a los programas cliente procesar las opciones usadas al ejecutar el
 * programa para obtener la direccion ip y el puerto del servidor.
 * Llama a exit(EXIT_FAILURE) si alguna de las opciones falta.
 */
void parse_options(int argc, char ** argv, struct sockaddr_in *servaddr);

#endif
