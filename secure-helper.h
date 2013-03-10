#ifndef SECURE_HELPER_H_
#define SECURE_HELPER_H_

// Simple structure to keep track of the handle, and
// of what needs to be freed later.
typedef struct {
    int socket;
    SSL *sslHandle;
    SSL_CTX *sslContext;
} connection;

#endif