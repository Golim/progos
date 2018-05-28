#ifndef LOGGER_h
#define LOGGER_h

#include "../util/util.h"

/**
 * check if it's a control message or a command to log
 * Return the return value of write_log function
 * */
int elaborate(msg *m);

#endif