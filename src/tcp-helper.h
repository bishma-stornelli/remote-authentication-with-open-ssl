#ifndef TCP_HELPER_H_
#define TCP_HELPER_H_

#include <arpa/inet.h>        /*  inet (3) funtions         */

#define LISTENQ        (1024)   /*  Backlog for listen()   */

int tcp_connect(struct sockaddr_in *servaddr);

int tcp_listen(struct sockaddr_in *servaddr);

#endif