/*
   1、用顺序表存储一些正整数，输入正整数表示插入数据（比如输入3表示插入3），输入负整数表示删除数据（比如输入-2表示删除2），输入字符表示退出程序。插入和删除的过程中保持该表递增有序。
*/

#include "seqlist.h"

int main(void)
{
      seq_plist sl;
      datatype data;
      int ret;

      init_seqlist(&sl);      //初始化顺序表
      while(1){
	    printf("请输入一个整数:");
	    ret = scanf("%d",&data);
	    if(ret == 0){   //输入为字符
		  printf("输如错误！Byebye...\n");
		  break;
	    }else if(data < 0){	    //输入为负数，删除对应的整数
		  del(sl,-data);
		  show_seqlist(sl);
	    }else{	//输入为整数，插入到顺序表中
		  insert(sl,data);
		  show_seqlist(sl);
	    }
      }
      return 0;
}

