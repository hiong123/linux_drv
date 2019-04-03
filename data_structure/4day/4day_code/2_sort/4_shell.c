#include <stdio.h>
#define N 10

void shellsort(int a[]);
void show(int a[]);
int main(void)
{
      int a[] = {50,36,66,76,95,12,25,36,24,8};

      show(a);
      shellsort(a);
      return 0;
}

void shellsort(int a[])
{
      int tmp,i,j,d;
      for(d = N/2; d > 0; d /= 2){
	    for(i = d;i < N; i++){
		  tmp = a[i];
		  for(j = i-d;j >= 0; j -= d)
			if(tmp < a[j])
			      a[j+d] = a[j];
			else
			      break;
		  a[j+d] = tmp;
	    }
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
