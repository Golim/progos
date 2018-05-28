#ifndef EXECUTE_h
#define EXECUTE_h

#include "../parser/parser.h"
#include "../sys_wrapper/sys_wrapper.h"

/**
 * execute a command formatted as a list of tokens
 * Return the return value of run_and_log function if everything works fine
 *        -1 otherwise
 * */
int execute(int s, int f);

#endif