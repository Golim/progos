#ifndef STATS_h
#define STATS_h

#include "../util/util.h"
#include "../config_output.h"

// Const Value
#define TIME_FACTOR (1000000)
#define COMMAND_MAX_LEN (255)
#define STATS_MAX_LEN (500)
#define SEP_MAX_LEN (20)
#define ARG_MAX_LEN (255)

// Functions
int stats(char *cmd, char *stat, char *sep, bool mu, bool names);
void send_to_logger(msg *m);

#endif
