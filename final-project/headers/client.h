#ifndef CLIENT_H
#define CLIENT_H
#include "deps.h"

void *client(void *param);
void sendMsgs(int sock, int receiver);
void produceMsg(int sig);

#endif