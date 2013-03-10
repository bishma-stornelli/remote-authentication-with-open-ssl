#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>

#include "openssl/ssl.h"
#include "openssl/err.h"

#include "options-server.h"   /*  parse options for servers */
#include "helper.h"           /*  our own helper functions  */
#include "usuarios.h"         /*  manejo de usuarios  */
#include "tcp-helper.h"
#include "secure-helper.h"

#define FAIL    -1
#define MAX_LINE           (1000)

SSL_CTX* InitServerCTX(void) {
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();  	/* load & register all cryptos, etc. */
    SSL_load_error_strings();   	/* load all error messages */
    ctx = SSL_CTX_new(SSLv23_server_method());   	/* create new context from method */
    
	if ( ctx == NULL ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
	
    return ctx;
}

void LoadCertificates(SSL_CTX *ctx, char* CertFile, char* KeyFile) {
	/* set the local certificate from CertFile */
    if (SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        abort();
    }
	
    /* set the private key from KeyFile (may be the same as CertFile) */
    if (SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    /* verify private key */
    if (!SSL_CTX_check_private_key(ctx)) {
        fprintf(stderr, "Private key does not match the public certificate\n");
        abort();
    }
}

void validate_user(Connection* con){
	char buffer[MAX_LINE];      /*  character buffer          */
	char *username;
	char *password;

	/* Ask username and password */
	sprintf(buffer, "Nombre de usuario: \n");
	ssl_write(con, buffer);
	username = ssl_read(con);
	printf("Nombre de usuario leido: '%s'\t", username);

	/* Ask password */
	sprintf(buffer, "Clave: \n");
	ssl_write(con, buffer);
	password = ssl_read(con);
	printf("Clave leida: '%s'\t", password);

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
	
	ssl_write(con, buffer);

	free(username);
	free(password);
}

int main(int argc, char **argv) {
	int       conn_s;                /*  connection socket         */
	struct    sockaddr_in servaddr;  /*  socket address structure  */
	Connection *c;

	parse_options(argc, argv, &servaddr); 

	c = malloc(sizeof(Connection));
	c->sslHandle = NULL;
	c->sslContext = NULL;

	c->socket = tcp_listen(&servaddr);

    SSL_library_init();
	
	/* initialize SSL */
	c->sslContext = InitServerCTX(); 

    LoadCertificates(c->sslContext, "../certificate/myserver.crt", "../certificate/myserver.key"); /* load certs */

	while ( 1 ) {
		/*  Wait for a connection, then accept() it  */
		if ( (conn_s = accept(c->socket, NULL, NULL) ) < 0 ) {
			fprintf(stderr, "Error calling accept()\n");
			exit(EXIT_FAILURE);
		}
		
        c->sslHandle = SSL_new(c->sslContext);	/* get new SSL state with context */
        SSL_set_fd(c->sslHandle, conn_s);    	/* set connection socket to SSL state */		
		
		if ( SSL_accept(c->sslHandle) == FAIL ) {     
			fprintf(stderr, "Error accepting SSL connection");
		} else {
			validate_user(c);
		}
		
		int sd = SSL_get_fd(c->sslHandle);  /* get socket connection */
		SSL_free(c->sslHandle);         	/* release SSL state */
		close(sd);
	}
}
