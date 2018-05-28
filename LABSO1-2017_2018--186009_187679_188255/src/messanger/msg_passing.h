#ifndef MSG_PASSING_h
#define MSG_PASSING_h

#include "../util/util.h"

#define EXTREF "/tmp/"
#define EXTID (99)

extern int elaborate(msg *m);

//============== client ============
/**
 * Initialize the client side of queue
 * Return OK_STATUS if it was correctly inizialized
 *        FAIL_STATUS otherwise
 * */
int init_client();
int send_msg(msg *m);

/**
 * delete the client's access to queue
 * */
int delete_client();

//============== server ============
/**
 * Initialize the client side of queue
 * Return OK_STATUS if it was correctly inizialized
 *        FAIL_STATUS otherwise
 * */
int init_server();

/**
 * Keep receiving messages until STOP_STATUS is detected
 * */
int start_listening();

/**
 * delete the server's access to queue
 * */
int delete_server();

#endif