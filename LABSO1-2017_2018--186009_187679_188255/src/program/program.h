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

/**
 * Generate daemon if it isn't running, and execute the command received
 * Return OK_STATUS if all ran fine
 *        EXIT_FAILURE otherwise
 * */
int run_program(char *cmd);

#endif
