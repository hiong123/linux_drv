#ifndef __SEQQUEUE_H__
#define __SEQQUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 8

typedef int datatype;

typedef struct seqqueue{
      datatype data[SIZE];
      int front,rear;
}seq_queue,*seq_pqueue;

extern void init_seqqueue(seq_pqueue * S);
extern bool in_seqqueue(seq_pqueue q,datatype d);
extern bool out_seqqueue(seq_pqueue q,datatype *d);
extern bool isfull_seqqueue(seq_pqueue s);
extern bool isempty_seqqueue(seq_pqueue s);
extern void show_seqqueue(seq_pqueue s);
#endif
