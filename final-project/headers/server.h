#ifndef SERVER_H
#define SERVER_H
#include "deps.h"

void server(void);
void *client(void *);
void *receiveMsg(void *newfd);

#endif

