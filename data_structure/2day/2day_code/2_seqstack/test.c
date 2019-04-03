#include "seqstack.h"

int main(void)
{
      seq_pstack s;
      int data,m;
      datatype t;

      init_seqstack(&s);
      printf("请输入十进制整数:");
      scanf("%d",&data);
      m = data;
      if(data < 0)
	    data = -data;

      while(data != 0){
	    push_seqstack(s,data%8);
	    data /= 8;
      }
      if(m < 0)
	    printf("十进制数%d转换成八进制数为:-0",m);
      else
	    printf("十进制数%d转换成八进制数为:0",m);
      while(!isempty_seqstack(s)){
	    pop_seqstack(s,&t);
	    printf("%d",t);
      }
      printf("\n");

      return 0;
}
