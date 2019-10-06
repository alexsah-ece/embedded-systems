#ifndef DEPS_H
#define DEPS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#include <ctype.h>
#include <sys/time.h>
#include <time.h>

#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <pthread.h>


#define PORT 2288
#define SENDER 8906
#define MAX_MSG_LENGTH 278
#define BUFFLENGTH 2000
#define LOG_BATCH_SIZE 10



char **IPs;
int ip_count;

int *IPsLastMsgSentIndex;
char **IPsLastMsgSent;
int *IPsToAEMs;

char **messageList;
int message_count;

char buff[BUFFLENGTH][MAX_MSG_LENGTH];
int count;
int fullBuffer;

char log_buffer[LOG_BATCH_SIZE][MAX_MSG_LENGTH];
int curr_log_count;

pthread_mutex_t buffer_mutex;
pthread_mutex_t fd_mutex;

void logger(char *receiveMsg, int flush_remaining);

#endif