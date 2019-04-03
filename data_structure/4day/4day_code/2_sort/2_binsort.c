#include <stdio.h>
#define N 8

void binsort(int a[]);
void show(int a[]);
int main(void)
{
      int a[] = {50,36,66,76,95,12,25,36};

      show(a);
      binsort(a);
      return 0;
}

void binsort(int a[])
{
      int tmp,i,j;
      int low,high,mid;
      for(i = 1;i < N; i++){
	    tmp = a[i];
	    //折半查找tmp的位置
	    for(low = 0,high = i-1; low <= high;){
		  mid = (low + high) / 2;
		  if(tmp > a[mid])
			low = mid +1;
		  else
			high = mid -1;
	    }
	    //从low开始的记录向后移动一个位置
	    for(j = i; j > low; j--)
		  a[j] = a[j-1];
	    //将tmp插入到low的位置
	    a[low] = tmp;
	    show(a);
      }
}

void show(int a[])
{
      int i;

      for(i = 0; i < N; i++)
	    printf("%d\t",a[i]);
      printf("\n");
}
