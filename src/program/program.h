#ifndef PROGRAM_h
#define PROGRAM_h

#include "../util/util.h"
#include "../stats/stats.h"
#include "../messanger/msg_passing.h"
#include "../config_output.h"
#include "../parser/parser.h"
#include "../sys_wrapper/sys_wrapper.h"

#define DAEMON_CH_DIR (TRUE)
#define DAEMON_CLOSE_OUT (TRUE)

extern int run_cmd(char *);

int run_program(char *cmd);

#endif
