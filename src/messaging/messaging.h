#define TMPLEN (100)

#define MSG_TEXT_LEN (255)
#define MSG_FILENAME_LEN (255)

#define EXTREF "./"
#define EXTID (12)

// Types
typedef struct mgs
{
    long mesg_type;
    char mesg_text[MSG_TEXT_LEN];
    char file_name[MSG_FILENAME_LEN];
} msg;

int msgkey(int mode);