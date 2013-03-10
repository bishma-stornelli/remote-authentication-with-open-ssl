#include <stdio.h>
#include "usuarios.h"

#define MAX_LINE 100

int main () {
	char username[MAX_LINE];
	char password[MAX_LINE];
	printf("Agregar nuevo usuario\n");
	
	printf("Nombre de usuario: ");
	scanf("%s", username);
	
	printf("Clave: ");
	scanf("%s", password);
	
	int res = guardar_usuario(username, password);
	switch(res) {
		case OK:
			printf("Usuario registrado\n");
			break;
		case ERROR_ARCHIVO:
			printf("Error: No se puede acceder al archivo de usuarios\n");
			break;
		case ERROR_NOMBRE:
			printf("Error: Ya existe un usuario de igual nombre\n");
			break;
	}
}