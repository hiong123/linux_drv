#include "search.h"

//成功返回记录位置，失败返回-1
int seqsearch(dtype a[],int n,int key)
{
      int i;

      for(i  = n-1; i >= 0; i--)
	    if(key == a[i].key)
		  return i;
      return -1;
}
