#ifndef PROGRAM_h
#define PROGRAM_h

#include "../util.h"
#include "../stats/stats.h"
#include "../message_passing/config.h"

#define DAEMON_CH_DIR (TRUE)
#define DAEMON_CLOSE_OUT (TRUE)

int run(int format, char *filename, char *cmd, bool mu);
void stop();
#endif
