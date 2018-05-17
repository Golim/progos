#ifndef STATS_h
#define STATS_h

#include "../util.h"

// Const Value
#define TIME_FACTOR (1000000)
#define COMMAND_MAX_LEN 255
#define STATS_MAX_LEN 255
#define ARG_MAX_LEN 255

// Functions
int stats(char *cmd, char *stat, char *sep, bool mu);
int separe_command_args(char *cmd, char *name, char *arg);
void send_to_logger(msg *m);
#endif
