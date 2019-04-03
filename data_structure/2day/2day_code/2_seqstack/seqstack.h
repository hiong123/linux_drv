#ifndef __SEQSTACK_H__
#define __SEQSTACK_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 8

typedef int datatype;

typedef struct seqstack{
      datatype data[SIZE];
      int top;
}seq_stack,*seq_pstack;

extern void init_seqstack(seq_pstack * S);
extern void push_seqstack(seq_pstack s,datatype d);
extern void pop_seqstack(seq_pstack s,datatype *d);
extern bool isfull_seqstack(seq_pstack s);
extern bool isempty_seqstack(seq_pstack s);
extern void show_seqstack(seq_pstack s);
#endif
