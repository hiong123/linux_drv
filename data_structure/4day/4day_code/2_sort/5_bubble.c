#include <stdio.h>
#define N 8

void bubblesort(int a[]);
void show(int a[]);
int main(void)
{
      int a[] = {50,36,66,76,95,12,25,36};

      show(a);
      bubblesort(a);
      return 0;
}

void bubblesort(int a[])
{
      int tmp,i,j;
      int flag;
      for(i = 0; i < N-1;i++){
	    flag = 0;
	    for(j = 0; j < N-1-i; j++)
		  if(a[j] > a[j+1]){
			tmp = a[j];
			a[j] = a[j+1];
			a[j+1] = tmp;
			flag = 1;
		  }
	    if(flag == 0)
		  break;
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
