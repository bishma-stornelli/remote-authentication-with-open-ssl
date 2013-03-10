#include "usuarios.h"

/**
 * Transforma el password a clave e inicializa el contexto de cifrado
 */
int iniciar_cifrado(EVP_CIPHER_CTX *e_ctx) {
	// Password y salt
	const char * password = PASSWORD;
    const unsigned char * salt = NULL;
	
	unsigned char key[32], iv[32];
	unsigned char * pass = (unsigned char *) password;
	int n = strlen(password);
	int nrounds = 5;
    
	// Generando clave a partir del password
	if (!EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, pass, n, nrounds, key, iv)) {
		return ERROR;
	}

	// Iniciando contexto cifrado
	EVP_CIPHER_CTX_init(e_ctx);
	EVP_EncryptInit_ex(e_ctx, EVP_aes_256_cbc(), NULL, key, iv);
}

/**
 * Transforma el password a clave e inicializa el contexto de decifrado
 */
int iniciar_descifrado(EVP_CIPHER_CTX *d_ctx) {
	// Password y salt
	const char * password = PASSWORD;
    const unsigned char * salt = NULL;
	
	unsigned char key[32], iv[32];
	unsigned char * pass = (unsigned char *) password;
	int n = strlen(password);
	int nrounds = 5;
    
	// Generando clave a partir del password
	if (!EVP_BytesToKey(EVP_aes_256_cbc(), EVP_sha1(), salt, pass, n, nrounds, key, iv)) {
		return ERROR;
	}

	// Iniciando contexto descifrar
	EVP_CIPHER_CTX_init(d_ctx);
	EVP_DecryptInit_ex(d_ctx, EVP_aes_256_cbc(), NULL, key, iv);
	return OK;
}

/**
 * Dado un texto claro, retorna el texto cifrado mediante AES
 */
char * cifrar(EVP_CIPHER_CTX *e, char *texto, int n) {
	iniciar_cifrado(e);

	int c_len = n + EVP_CIPHER_CTX_block_size(e);
	int f_len = 0;
	unsigned char *texto_cifrado = malloc(c_len);

	EVP_EncryptInit_ex(e, NULL, NULL, NULL, NULL);
	EVP_EncryptUpdate(e, texto_cifrado, &c_len, texto, n);
	EVP_EncryptFinal_ex(e, texto_cifrado + c_len, &f_len);

	EVP_CIPHER_CTX_cleanup(e);
	return texto_cifrado;
}

/**
 * Dado un texto cifrado, retorna el texto claro mediante AES
 */
char * descifrar(EVP_CIPHER_CTX *e, char *texto_cifrado, int n) {
	iniciar_descifrado(e);
	
	int p_len = n;
	int f_len = 0;
	char *texto_plano = malloc(p_len + EVP_CIPHER_CTX_block_size(e));

	EVP_DecryptInit_ex(e, NULL, NULL, NULL, NULL);
	EVP_DecryptUpdate(e, texto_plano, &p_len, texto_cifrado, n);
	EVP_DecryptFinal_ex(e, texto_plano + p_len, &f_len);

	EVP_CIPHER_CTX_cleanup(e);
	return texto_plano;
}

/**
 * Guarda el nombre de usuario y su password cifrado en el archivo USUARIOS
 */
int guardar_usuario(char * nombre, char * password) {
	FILE * usuarios;
	usuarios = fopen(USUARIOS, "a+");

	// Error al abrir el archivo
	if (!usuarios) {
		return ERROR_ARCHIVO;
	}

	// Verificando si el usuario ya existe
	char linea[1024];
	char * token;
	while (feof(usuarios) == 0) {
        fgets(linea, 1024, usuarios);
		token = strtok(linea, " - ");
	
		// Si existe, salir
		if (!strcmp(token, nombre)) {
			fclose(usuarios);
			return ERROR_NOMBRE;
		}
	}
	
	// Cifrando password
	EVP_CIPHER_CTX en;	
	char * pass_cifrado;
	int n = strlen(password) + 1;
	pass_cifrado = cifrar(&en, password, n);
	
	// Escribiendo en archivo
	fprintf(usuarios, "%s - %s\n", nombre, pass_cifrado);

	// Liberando memoria
	free(pass_cifrado);
	fclose(usuarios);
	return OK;
}

/**
 * Verifica si la combinacion nombre y password es correcta
 */
int comparar_usuario(char * nombre, char * password) {
	FILE * usuarios;
	usuarios = fopen(USUARIOS, "r");
	
	if (!usuarios) {
		return ERROR_ARCHIVO;
	}
	
	// Leyendo archivo de usuarios
	char linea[1024]; 
	char * token;
	while (feof(usuarios) == 0) {
        fgets(linea, 1024, usuarios);

		// Eliminando salto de linea
		char *pos;
		if ((pos = strchr(linea, '\n')) != NULL) {
			*pos = '\0';
		}
		
		// Nombre de usuario
		token = strtok(linea, " - ");
		if (!token) continue;
		
		if (!strcmp(token, nombre)) {		
			// Password cifrado
			token = strtok(NULL, " - ");
			int n = strlen(token);
			
			// Descifrando y comparando password
			EVP_CIPHER_CTX en;
			char * pass = descifrar(&en, token, n);
			if (!strcmp(pass, password)) {
				free(pass);
				fclose(usuarios);
			
				return OK;
			}
			
			free(pass);
			fclose(usuarios);
			
			return ERROR_PASSWORD;
		}
	}
	
	fclose(usuarios);
	return ERROR_NOMBRE;
}