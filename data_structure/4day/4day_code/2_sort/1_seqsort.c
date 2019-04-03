#include <stdio.h>
#define N 8

void seqsort(int a[]);
void show(int a[]);
int main(void)
{
      int a[] = {50,36,66,76,95,12,25,36};

      show(a);
      seqsort(a);
      return 0;
}

void seqsort(int a[])
{
      int tmp,i,j;
      for(i = 1;i < N; i++){
	    tmp = a[i];
	    for(j = i-1;j >= 0; j--)
		  if(tmp < a[j])
			a[j+1] = a[j];
		  else
			break;
	    a[j+1] = tmp;
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
