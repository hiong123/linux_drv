#ifndef _SEQLIST_H__
#define _SEQLIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define SIZE 10

typedef int datatype;
typedef struct seqlist{
      datatype data[SIZE];
      int last;
}seq_list,*seq_plist;

extern void init_seqlist(seq_plist *L);
extern void insert_seqlist(seq_plist sl,int i,datatype d);
extern void insert(seq_plist sl,datatype d);
extern void del_seqlist(seq_plist sl,int i);
extern void del(seq_plist sl,datatype d);
extern bool isempty_seqlist(seq_plist sl);
extern bool isfull_seqlist(seq_plist sl);
extern void show_seqlist(seq_plist sl);
#endif
