#ifndef PROGRAM_h
#define PROGRAM_h

#include "../util.h"
#include "../stats/stats.h"
#include "../message_passing/config.h"

int run(int format, char *filename, char *cmd, bool mu);
void stop();
#endif
