#define OPERATOR (1)
#define COMMAND (2)
#define OPERATOR (1)

int parse(char *cmd);
int tokenize(char *cmd, int start, int finish);
int is_control_operator(char *cmd, int i);
int is_meta_character(char c);
//TODO:
int is_valid_command(char * cmd)
{
  return TRUE;
}
bool is_valid_filename (char * fn)
{
  return TRUE;
}
typedef struct exp_token
{
  int type;
  char value[50];
} exp_token;