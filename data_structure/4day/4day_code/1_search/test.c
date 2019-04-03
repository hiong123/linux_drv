#include "search.h"

void show(dtype a[],int n)
{
      int i;
      for(i = 0;i < n; i++)
	    printf("%d\t",a[i].key);
      printf("\n");
}

int main(void)
{
      dtype a[] = {{1},{2},{3},{4},{5},{6},{7},{8}};
      int key,ret;
      char ch;
      
      show(a,8);
      while(1){
	    printf("请输入要查找的记录的关键字key:");
	    scanf("%d",&key);
	    ret = binsearch(a,8,key);
	    if(ret == -1){   //查找失败
		  printf("要查找的记录不存在!\n");
	    }else{	//查找成功
		  printf("您要查找的记录在%d位置!\n",ret);
	    }
	    printf("是否继续查找?(Y/y)\n");
	    while(getchar() != '\n');   //清缓冲区
	    scanf("%c",&ch);
	    if(ch != 'Y' && ch != 'y')
		  break;
      }
      return 0;
}
