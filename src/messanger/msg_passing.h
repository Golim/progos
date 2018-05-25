#ifndef MSG_PASSING_h
#define MSG_PASSING_h

#include "../util/util.h"

#define EXTREF "/tmp/"
#define EXTID (99)

extern int elaborate(msg *m);

int init_client();
int send_msg(msg *m);
int delete_client();

int init_server();
int start_listening();
int delete_server();

#endif