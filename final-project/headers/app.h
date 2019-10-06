#ifndef APP_H
#define APP_H
#include "deps.h"
#include "client.h"
#include "server.h"

void initialize_vars(void);
void initialize_addresses(char *file);
void initialize_messages(char *file);
void catch_int_term(int signal);

#endif
