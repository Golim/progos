// Unique references for queue
#define EXTREF "./"
#define EXTID  (12)

#define TMPLEN (100)
#define MAXLEN 200

#include <stdio.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

typedef char* string;
char *tmpfiletpl="queue.tmp";
const string DEF_TXT = "log.txt";
const string DEF_CSV = "log.csv";

int msgkey(int mode) { // mode == 0 for READ, 1 for write
	char tmpfile[TMPLEN];
	int ft;
	char cwd[1024];
	char *cnt="---";
	
	getcwd(cwd, sizeof(cwd));
	
	strcpy(tmpfile, EXTREF);
	strcat(tmpfile, tmpfiletpl);
	ft = open(tmpfile, O_WRONLY | O_CREAT, 0777);
	
	write(ft, cnt, strlen(cnt)+1);
	
	close(ft);
	
	remove(tmpfile);
	
	// ftok to generate unique key
	int key = ftok(tmpfile, EXTID);
	if (mode==0) remove(tmpfile);
	//printf("extref='%s', tmpfile='%s' (ft=%d), extid='%d', msgkey='%d'\n", EXTREF, tmpfile, ft, EXTID, key);
	return (key);
}

/*
if message type = 1 --> txt
if message type = 2 --> csv
if name file is empty keep the default log file
*/
struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAXLEN];
	char file_name[MAXLEN];
};// message

void write_log(struct mesg_buffer *msg)
{
	FILE* loggher_logs = fopen("/tmp/logger_logs.txt", "a");
	FILE* f;

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	fprintf(loggher_logs, "%d:%d:%d) message received \"%s\"\n", tm.tm_hour, tm.tm_min, tm.tm_sec, msg->mesg_text);
	
	if(strlen(msg->mesg_text) > 0) 
	{
		if(strstr(msg->mesg_text, "quit") != NULL || strstr(msg->mesg_text, "kill") != NULL) 
		{
			fprintf(loggher_logs, "%d:%d:%d) logger terminated\n", tm.tm_hour, tm.tm_min, tm.tm_sec, msg->mesg_text);
			exit(0);
		} 
		else 
		{
			if(!strlen(msg->file_name) > 0)
			{
				switch(msg->mesg_type)
				{
					case 1:
						f = fopen(DEF_TXT, "a");
						fprintf(loggher_logs, "%d:%d:%d) message recorded in %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, msg->mesg_text, msg->file_name);
						break;
					case 2:
						f = fopen(DEF_CSV, "a");
						fprintf(loggher_logs, "%d:%d:%d) message recorded in %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, msg->mesg_text, msg->file_name);
						break;
					default:
						fprintf(loggher_logs, "%d:%d:%d) ERR: mesg_type not recognized!\n", tm.tm_hour, tm.tm_min, tm.tm_sec, msg->mesg_text);
						break;
				}//end switch
			}
			else
			{
				//TO DO: extract file path from file_name
				switch(msg->mesg_type)
				{
					case 1:
						f = fopen(DEF_TXT, "a");
						fprintf(loggher_logs, "%d:%d:%d) message recorded in %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, msg->mesg_text, msg->file_name);
						break;
					case 2:
						f = fopen(DEF_CSV, "a");
						fprintf(loggher_logs, "%d:%d:%d) message recorded in %s\n", tm.tm_hour, tm.tm_min, tm.tm_sec, msg->mesg_text, msg->file_name);
						break;
					default:
						fprintf(loggher_logs, "%d:%d:%d) ERR: mesg_type not recognized!\n", tm.tm_hour, tm.tm_min, tm.tm_sec, msg->mesg_text);
						break;
				}//end switch
			}//end if(!strlen(msg->file_name) > 0)
		}//end if(strstr...!=NULL)
	}//end if(strlen(message.mesg_text) > 0) 

}//end write_log