#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "secure-helper.h"
#include "tcp-helper.h"

// Establish a Connection using an SSL layer
Connection *ssl_connect(struct sockaddr_in *servaddr) {
  Connection *c;

  c = malloc(sizeof(Connection));
  c->sslHandle = NULL;
  c->sslContext = NULL;

  c->socket = tcp_connect(servaddr);
  if (c->socket) {
      // Register the error strings for libcrypto & libssl
      SSL_load_error_strings();
      // Register the available ciphers and digests
      SSL_library_init();

      // New context saying we are a client, and using SSL 2 or 3
      c->sslContext = SSL_CTX_new(SSLv23_client_method());
      if (c->sslContext == NULL)
        ERR_print_errors_fp(stderr);

      // Create an SSL struct for the Connection
      c->sslHandle = SSL_new(c->sslContext);
      if (c->sslHandle == NULL)
        ERR_print_errors_fp(stderr);

      // Connect the SSL struct to our Connection
      if (!SSL_set_fd(c->sslHandle, c->socket))
        ERR_print_errors_fp(stderr);

      // Initiate SSL handshake
      if (SSL_connect(c->sslHandle) != 1)
        ERR_print_errors_fp(stderr);
  } else {
      perror("Connect failed");
  }
  return c;
}

// Disconnect & free Connection struct
void ssl_disconnect(Connection *c) {
  if (c->socket)
    close(c->socket);
  if (c->sslHandle)
    {
      SSL_shutdown(c->sslHandle);
      SSL_free(c->sslHandle);
    }
  if (c->sslContext)
    SSL_CTX_free(c->sslContext);

  free(c);
}

// Read all available text from the Connection
char *ssl_read(Connection *c) {
  const int readSize = 1024;
  char *rc = NULL;
  int received, count = 0;
  char buffer[1024];

  if (c) {
      while (1) {
          if (!rc)
            rc = malloc(readSize * sizeof(char) + 1);
          else
            rc = realloc(rc, (count + 1) * readSize * sizeof(char) + 1);

          received = SSL_read(c->sslHandle, buffer, readSize);
          buffer[received] = '\0';

          if (received > 0)
            strcat(rc, buffer);

          if (received < readSize)
            break;

          ++count;
      }
  }

  return rc;
}

// Write text to the Connection
void ssl_write(Connection *c, char *text) {
  if (c)
    SSL_write(c->sslHandle, text, strlen(text));
}