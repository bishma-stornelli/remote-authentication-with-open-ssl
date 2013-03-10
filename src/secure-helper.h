#ifndef SECURE_HELPER_H_
#define SECURE_HELPER_H_

#include <openssl/ssl.h>
#include <arpa/inet.h>        /*  inet (3) funtions         */

// Simple structure to keep track of the handle, and
// of what needs to be freed later.
typedef struct {
    int socket;
    SSL *sslHandle;
    SSL_CTX *sslContext;
} Connection;

Connection *ssl_connect(struct sockaddr_in *servaddr);

void ssl_disconnect(Connection *c);

char *ssl_read (Connection *c);

void ssl_write (Connection *c, char *text);

#endif