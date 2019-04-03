#include "linkstack.h"

int main(void)
{
      link_pstack top;	      //栈顶指针
      int data,m;
      datatype t;

      init_linkstack(&top);     //初始化链栈
      printf("请输入十进制整数:");
      scanf("%d",&data);
      m = data;
      if(data < 0)
	    data = -data;

      while(data != 0){
	    push_linkstack(&top,data%8);	  
	    data /= 8;
      }
      if(m < 0)
	    printf("十进制数%d转换成八进制数为:-0",m);
      else
	    printf("十进制数%d转换成八进制数为:0",m);
      while(!isempty_linkstack(top)){
	    pop_linkstack(&top,&t);
	    printf("%d",t);
      }
      printf("\n");

      return 0;
}
