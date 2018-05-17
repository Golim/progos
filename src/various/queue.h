#include "../util.h"
#include "parser.h"

void init (queue Q);
bool enqueue(queue Q , exp_token );
bool first(queue Q , exp_token *);
bool dequeue(queue Q);
void print (queue Q);

typedef struct node {
exp_token val;
node * next;
} node;





