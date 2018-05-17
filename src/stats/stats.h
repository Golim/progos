#ifndef STATS_h
#define STATS_h

#include "../util.h"

// Const Value
const long TIME_FACTOR = 1000000;
const long COMMAND_MAX_LEN = 255;
const long STATS_MAX_LEN = 500;
const long SEP_MAX_LEN = 20;
const long ARG_MAX_LEN = 255;

// Functions
int stats(char *cmd, char *stat, char *sep, bool mu, bool names);
int separe_command_args(char *cmd, char *name, char *arg);
void send_to_logger(msg *m);
void print_usage();
void print_help();
void tolowercase(char *s);

#endif
