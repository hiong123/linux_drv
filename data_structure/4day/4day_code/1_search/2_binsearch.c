#include "search.h"

//成功返回记录位置，失败返回-1
int binsearch(dtype a[],int n,int key)
{
      int low,high,mid;

      low = 0;
      high = n-1;
      while(low <= high){
	    mid = (low + high) / 2;
	    if(key == a[mid].key)
		  return mid;
	    else if(key > a[mid].key)
		  low = mid + 1;
	    else
		  high = mid -1;
      }
      return -1;
}
