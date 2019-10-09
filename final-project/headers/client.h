#ifndef CLIENT_H
#define CLIENT_H
#include "deps.h"

#define CLIENT_SENT_PORT 2288
#define CLIENT_CONNECTION_ATTEMPT_PERIOD 10
#define PRODUCE_MSG_INTERVAL 15
#define PRODUCE_MSG_RANGE 10

struct msg_struct {
    uint32_t sender;
    uint32_t receiver;
    uint64_t ts;
    char payload[256];
};

void *client(void *param);
void send_messages(int sock, int receiver);
void produce_msg(int sig);
struct msg_struct create_msg();

#endif