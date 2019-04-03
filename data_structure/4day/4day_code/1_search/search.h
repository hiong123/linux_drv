#ifndef __SEARCH_H__
#define __SEARCH_H__

#include <stdio.h>

typedef struct A{
      int key;
      /*
	   其他属性  
       */
}dtype;

extern int seqsearch(dtype a[],int n,int key);
extern int binsearch(dtype a[],int n,int key);
#endif
