#ifndef NULL
#define NULL 0
#endif

#include "parser.h"
#include "queue.h"

typedef struct queue
{
  node *tail;
  node *head;
} queue;

bool emptyp(queue Q)
{
  return (Q.head == NULL);
}

void init(queue Q)
{
  Q.head = NULL;
}

bool enqueue(queue Q, exp_token n)
{
  // memo: "new (nothrow) ..." restituisce NULL
  // senza generare eccezioni se l'allocazione non va a buon fine
  bool res;
  node *np = malloc(sizeof(node));
  if (np == NULL)
    res = FALSE;
  else
  {
    np->val = n;
    np->next = NULL;
    if (emptyp(Q))
      Q.head = np;
    else
      Q.tail->next = np;
    Q.tail = np;
    res = TRUE;
  }
  return res;
}

bool first(queue Q, exp_token *n)
{
  bool res;
  if (emptyp(Q))
    res = FALSE;
  else
  {
    *n = Q.head->val;
    res = TRUE;
  }
  return res;
}

bool dequeue(queue Q)
{
  bool res;
  if (emptyp(Q))
    res = FALSE;
  else
  {
    node *first;
    first = Q.head;
    Q.head = Q.head->next;
    free(first);
    res = TRUE;
  }
  return res;
}

void print(queue Q)
{
  if (!emptyp(Q))
  {
    node *first = Q.head;
    do
    {
      if (first->val.type == COMMAND)
      {
        printf("C:[%s]  ",first->val.value);
      }
      else if (first->val.type == OPERATOR)
      {
        printf("Op:[%s]  ", first->val.value);
      }

      first = first->next;
    } while (first != NULL);
  }
}
