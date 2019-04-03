#include "seqlist.h"

//初始化
void init_seqlist(seq_plist *L)
{
      //申请顺序表空间
      *L = (seq_plist)malloc(sizeof(seq_list));
      if(NULL == *L){
	    perror("malloc"); //打印错误原因
	    exit(1);
      }
      //对空间进行初始化
      (*L)->last = -1;
}
//插入
void insert_seqlist(seq_plist sl,int i,datatype d)
{
      int j;
      for(j = sl->last; j >= i; j--)
	    sl->data[j+1] = sl->data[j];
      sl->data[i] = d;
      //last+1
      sl->last++;
}
void insert(seq_plist sl,datatype d)
{
      int i;
      if(isfull_seqlist(sl)){
	    printf("顺序表已满!\n");
	    return;
      }
      //寻找插入d的位置
      for(i = 0; i <= sl->last;i++)
	    if(d < sl->data[i])
		  break;
      //将d插入到i的位置
      insert_seqlist(sl,i,d);
}
//删除
void del_seqlist(seq_plist sl,int i)
{
      int j;
      for(j = i; j < sl->last; j++)
	    sl->data[j] = sl->data[j+1];
      //last减1
      sl->last--;
}
void del(seq_plist sl,datatype d)
{
      int i;
      if(isempty_seqlist(sl)){
	    printf("顺序表为空!\n");
	    return ;
      }
      //在顺序表中查找d
      for(i = 0; i <= sl->last; i++)
	    if(d == sl->data[i])
		  break;
      if(i > sl->last){   //d不存在
	    printf("记录%d不在顺序表中!",d);
	    return ;
      }
      //d在i的位置
      del_seqlist(sl,i);
}
//判断是否为空
bool isempty_seqlist(seq_plist sl)
{
      if(sl->last == -1)
	    return true;
      else
	    return false;
}
//判断是否满
bool isfull_seqlist(seq_plist sl)
{
      if(sl->last == SIZE-1)
	    return true;
      else
	    return false;
}
//遍历
void show_seqlist(seq_plist sl)
{
      int i;
      for(i = 0; i <= sl->last;i++)
	    printf("%d\t",sl->data[i]);
      printf("\n");
}
