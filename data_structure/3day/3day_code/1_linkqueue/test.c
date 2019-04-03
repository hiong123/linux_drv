/*
 3、分别用循环队列实现如下功能：用户从键盘输入整数，程序将其入队，用户输入字母，程序将队头元素出队，并在每一次出队和入队之后打印队列元素。
 */
#include "linkqueue.h"

int main(void)
{
      link_pqueue q;      //定义指向队列的指针q
      int ret;
      datatype data,t;
#if 0
      init_linkqueue(&q);      //初始化队列
#else
      q = init_linkqueue();
#endif
      while(1){
	    printf("请输入一个整数:");
	    ret = scanf("%d",&data);
	    if(ret == 1){  //输入为整数，入队
		  if(in_linkqueue(q,data))
			show_linkqueue(q);
	    }else{   //输入为字符，出队
		  if(out_linkqueue(q,&t)){
			printf("out is :%d\n",t);
			show_linkqueue(q);
		  }
		  while(getchar() != '\n');   //清空缓冲区
	    }
      }

          return 0;
}
