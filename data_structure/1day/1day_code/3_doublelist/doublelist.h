#ifndef _DOUBLELIST_H__
#define _DOUBLELIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int datatype;
typedef struct doublelist{
      datatype data;
      struct doublelist *next,*prev;
}double_list,*double_plist;

extern void init_doublelist(double_plist *L);
extern bool isempty_doublelist(double_plist sl);
extern void insert_doublelist(double_plist h,double_plist new);
extern void insert_doublelist_post(double_plist h,double_plist new);
extern void create(double_plist h);
extern void sort(double_plist h);
extern void del_doublelist(double_plist p);
extern void del_doublelist_pre(double_plist p);
extern void del_doublelist_post(double_plist p);
extern void show_doublelist(double_plist h);
#endif
