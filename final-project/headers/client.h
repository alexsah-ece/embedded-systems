#ifndef CLIENT_H
#define CLIENT_H
#include "deps.h"

#define CLIENT_SENT_PORT 2288

void *client(void *param);
void send_messages(int sock, int receiver);
void produce_msg(int sig);

#endif