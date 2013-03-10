#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>

#define USUARIOS "./usuarios.txt"
#define PASSWORD "yo_soy_la_clave_secreta"

#define OK 1
#define ERROR 2
#define ERROR_ARCHIVO 3
#define ERROR_NOMBRE 4
#define ERROR_PASSWORD 5


/**
 * Guarda el nombre de usuario y su password cifrado en el archivo USUARIOS
 */
int guardar_usuario(char * nombre, char * password);

/**
 * Verifica si la combinacion nombre y password es correcta
 */
int comparar_usuario(char * nombre, char * password);