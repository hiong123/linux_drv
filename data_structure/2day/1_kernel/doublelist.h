#ifndef _DOUBLELIST_H__
#define _DOUBLELIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

typedef int datatype;
typedef struct doublelist{
      datatype data;
      struct list_head list;  //内核链表结点
}double_list,*double_plist;

extern void init_doublelist(double_plist *L);
extern void create(double_plist h);
extern void sort(double_plist h);
extern void show_doublelist(double_plist h);
#endif
