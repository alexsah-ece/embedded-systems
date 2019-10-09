#ifndef SERVER_H
#define SERVER_H
#include "deps.h"

#define SERVER_LISTEN_PORT 2288

void server(void);
void *client(void *);
void *received_msg(void *newfd);

#endif

