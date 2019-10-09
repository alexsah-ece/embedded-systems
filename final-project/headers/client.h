#ifndef CLIENT_H
#define CLIENT_H
#include "deps.h"

#define CLIENT_SENT_PORT 2288
#define CLIENT_CONNECTION_ATTEMPT_PERIOD 10
#define PRODUCE_MSG_INTERVAL 15
#define PRODUCE_MSG_RANGE 10

void *client(void *param);
void send_messages(int sock, int receiver);
void produce_msg(int sig);

#endif