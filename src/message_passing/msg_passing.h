#ifndef MSG_PASSING_h
#define CONFIMSG_PASSING_hG_h

#include "../util/util.h"

//TODO: VA bene questo oppure bisogna cambiare?!
#define EXTREF "./"
#define EXTID (100)

extern int elaborate(msg *m);

int init_client();
int send_msg(msg *m);
int delete_client();

int init_server();
int start_listening();
int delete_server();

#endif