// Const Value
const long TIME_FACTOR = 1000000;
const long COMMAND_MAX_LEN = 255;
const long STATS_MAX_LEN = 255;
const long ARG_MAX_LEN = 255;

// Functions
int stats(char *cmd, char *stat, char *sep);
int separe_command_args(char *cmd, char *name, char *arg);
void send_to_logger(msg *m);
void print_usage();
void print_help();
