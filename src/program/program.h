#ifndef PROGRAM_h
#define PROGRAM_h

#include "../util.h"
#include "../stats/stats.h"
#include "../message_passing/config.h"
#include "../config_output.h"
#include "../parser/parser.h"

#define DAEMON_CH_DIR (TRUE)
#define DAEMON_CLOSE_OUT (TRUE)

int run(char *cmd);
void stop();

extern int esegui_programma(char*);
#endif
