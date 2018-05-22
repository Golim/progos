#ifndef PROGRAM_h
#define PROGRAM_h

#include "../util/util.h"
#include "../stats/stats.h"
#include "../message_passing/msg_passing.h"
#include "../config_output.h"
#include "../parser/parser.h"

#define DAEMON_CH_DIR (TRUE)
#define DAEMON_CLOSE_OUT (TRUE)

extern int esegui_programma(char *);

int run_program(char *cmd);

#endif
