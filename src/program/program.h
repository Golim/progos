#ifndef PROGRAM_h
#define PROGRAM_h

#ifndef UTIL_h
#include "../util.h"
#endif

#ifndef STATS_h
#include "../stats/stats.h"
#endif

#ifndef CONFIG_h
#include "../message_passing/config.h"
#endif

int run(int format, char *filename, char *cmd);
void stop();
#endif