#ifndef STATS_h
#define STATS_h

#include "../util/util.h"
#include "../config_output.h"

// Const Value
#define TIME_FACTOR (1000000)
#define COMMAND_MAX_LEN (255)
#define STATS_MAX_LEN (500)
#define ARG_MAX_LEN (255)

/**
 * calcoulate the statistics of a single command with one or more parameters
 * Return: the result code of the execution of the received command
 * */
int stats(char *cmd, char *stat, char *sep, bool mu, bool names);

#endif
