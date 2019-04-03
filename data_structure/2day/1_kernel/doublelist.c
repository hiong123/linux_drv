#include "doublelist.h"

//初始化
void init_doublelist(double_plist *L)
{
      *L = (double_plist)malloc(sizeof(double_list));
      if(NULL == *L){
	    perror("malloc");
	    exit(1);
      }
      INIT_LIST_HEAD(&(*L)->list);  //初始化头结点
}
void create(double_plist h)
{
      int n,i;
      double_plist new;
      printf("请输入整数的个数:");
      scanf("%d",&n);
      for(i = 0;i < n; i++){
	    new= (double_plist)malloc(sizeof(double_list));
	    if(NULL == new){
		  perror("malloc");
		  exit(1);
	    }
	    printf("请输入一个整数:");
	    scanf("%d",&new->data);
	    list_add_tail(&new->list,&h->list);
	    show_doublelist(h);
      }
}

void sort(double_plist h)
{
      double_plist p,q;
      p = list_entry(h->list.prev,double_list,list);
      while(p != h){
	    if(p->data % 2 != 0){ //奇数，p向前移动
		  p = list_entry(p->list.prev,double_list,list);
	    }else{//偶数，将p指向的结点剪切，插入到头结点前
		  q = p;
		  p = list_entry(p->list.prev,double_list,list);

		  list_move_tail(&q->list,&h->list);
		  show_doublelist(h);
	    }
      }
}
//遍历
void show_doublelist(double_plist h)
{
      double_plist p;

      list_for_each_entry(p,&h->list,list)
	    printf("%d\t",p->data);
      printf("\n");
}
