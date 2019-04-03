/*
 3、分别用循环队列实现如下功能：用户从键盘输入整数，程序将其入队，用户输入字母，程序将队头元素出队，并在每一次出队和入队之后打印队列元素。
 */
#include "seqqueue.h"

int main(void)
{
      seq_pqueue q;      //定义指向队列的指针q
      int ret;
      datatype data,t;

      init_seqqueue(&q);      //初始化队列
      while(1){
	    printf("请输入一个整数:");
	    ret = scanf("%d",&data);
	    if(ret == 1){  //输入为整数，入队
		  if(in_seqqueue(q,data))
			show_seqqueue(q);
	    }else{   //输入为字符，出队
		  if(out_seqqueue(q,&t)){
			printf("out is :%d\n",t);
			show_seqqueue(q);
		  }
		  while(getchar() != '\n');   //清空缓冲区
	    }
      }

          return 0;
}
