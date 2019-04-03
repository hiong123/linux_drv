#ifndef _LINKLIST_H__
#define _LINKLIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int datatype;
typedef struct linklist{
      datatype data;
      struct linklist *next;
}link_list,*link_plist;

extern void init_linklist(link_plist *L);
extern void insert_linklist(link_plist p,link_plist new);
extern void create(link_plist h);
extern void sort(link_plist h);
extern void del_linklist(link_plist p);
extern void del(link_plist h,datatype d);
extern bool isempty_linklist(link_plist sl);
extern void show_linklist(link_plist h);
#endif
