#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <string.h>
#include <unistd.h>
#define MAXLEN (100)
 
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAXLEN];
} message;

int check_daemon_exist();
int check_writer_exist();
 
int main() {
    long r;
    
    if(check_daemon_exist() == 0){
		printf("Daemon is not running\n");
		exit(1);
	}
    
    while(1){
		
		key_t key;
		int msgid;
		char msgtxt[MAXLEN];
	 
		key = msgkey(1);
	 
		msgid = msgget(key, 0666 | IPC_CREAT);
		message.mesg_type = 1;

		printf("Input data: ");
		r=(long)fgets(msgtxt, MAXLEN, stdin);
		
		strcpy(message.mesg_text, msgtxt);
		msgsnd(msgid, &message, sizeof(message), 0);
		printf("\n");
		
		if(strstr(msgtxt, "quit") != NULL){
			exit(0);
		}
	}
    return 0;
}

int check_daemon_exist(){
	int size = 0;
	
	system("ps -xj | grep -v grep | grep firstdaemon >> /tmp/.check");
	FILE *fp = fopen("/tmp/.check", "r");
	if (fp != NULL) {
		fseek (fp, 0, SEEK_END);
		size = ftell(fp);
	}
	fclose(fp);
	system("rm /tmp/.check");
	
	return size;
}

int check_writer_exist(){
	int size = 0;
	
	system("ps -xj | grep -v grep | grep writer >> /tmp/.check");
	FILE *fp = fopen("/tmp/.check", "r");
	if (fp != NULL) {
		fseek (fp, 0, SEEK_END);
		size = ftell(fp);
	}
	fclose(fp);
	system("rm /tmp/.check");
	
	return size;
}
