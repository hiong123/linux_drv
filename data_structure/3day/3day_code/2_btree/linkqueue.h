#ifndef __LINKQUEUE_H__
#define __LINKQUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "btree.h"
#define SIZE 8

typedef btree_pnode datatype;

typedef struct listnode{
      datatype data;
      struct listnode *next;
}list_node,*list_pnode;

typedef struct linkqueue{
      list_pnode front,rear;
}link_queue,*link_pqueue;

#if 0
extern void init_linkqueue(link_pqueue * S);
#else
extern link_pqueue init_linkqueue(void);
#endif
extern bool in_linkqueue(link_pqueue q,datatype d);
extern bool out_linkqueue(link_pqueue q,datatype *d);
extern bool isempty_linkqueue(link_pqueue s);
//extern void show_linkqueue(link_pqueue s);
#endif
