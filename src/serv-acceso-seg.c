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

#define FAIL    -1
#define MAX_LINE           (1000)

SSL_CTX* InitServerCTX(void) {   
	SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();  	/* load & register all cryptos, etc. */
    SSL_load_error_strings();   	/* load all error messages */
    method = SSLv2_server_method(); /* create new server-method instance */
    ctx = SSL_CTX_new(method);   	/* create new context from method */
    
	if ( ctx == NULL ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
	
    return ctx;
}

void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile) {
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

void validate_user(SSL* ssl){
	char buffer[MAX_LINE];      /*  character buffer          */
	char username[MAX_LINE];
	char password[MAX_LINE];

	/* Ask username and password */
	sprintf(buffer, "Nombre de usuario: \n");
	SSL_write(ssl, buffer, strlen(buffer));
	SSL_read(ssl, username, MAX_LINE-1);

	/* Ask password */
	sprintf(buffer, "Clave: \n");
	SSL_write(ssl, buffer, strlen(buffer));
	SSL_read(ssl, password, MAX_LINE-1);
		
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
	
	SSL_write(ssl, buffer, strlen(buffer));
}

int main(int argc, char **argv) { 
	int       list_s;                /*  listening socket          */
	int       conn_s;                /*  connection socket         */
	struct    sockaddr_in servaddr;  /*  socket address structure  */

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

	SSL_CTX *ctx;
    SSL_library_init();
	
	/* initialize SSL */
	ctx = InitServerCTX();        			
    LoadCertificates(ctx, "mycert.pem", "mycert.pem"); /* load certs */

	while ( 1 ) {
		/*  Wait for a connection, then accept() it  */
		if ( (conn_s = accept(list_s, NULL, NULL) ) < 0 ) {
			fprintf(stderr, "Error calling accept()\n");
			exit(EXIT_FAILURE);
		}
		
        SSL *ssl = SSL_new(ctx);	/* get new SSL state with context */
        SSL_set_fd(ssl, conn_s);    /* set connection socket to SSL state */		
		
		if ( SSL_accept(ssl) == FAIL ) {     
			printf("Error");
		} else {
			validate_user(ssl);
		}
			
		int sd = SSL_get_fd(ssl);  /* get socket connection */
		SSL_free(ssl);         /* release SSL state */
		close(sd);
		
		/*  Close the connected socket  */

		if ( close(conn_s) < 0 ) {
			fprintf(stderr, "ECHOSERV: Error calling close()\n");
			exit(EXIT_FAILURE);
		}
	}
}
