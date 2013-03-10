#ifndef TCP_HELPER_H_
#define TCP_HELPER_H_

#include <arpa/inet.h>        /*  inet (3) funtions         */

int tcp_connect(struct sockaddr_in *servaddr);

#endif