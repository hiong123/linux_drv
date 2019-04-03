#include "doublelist.h"

//初始化
void init_doublelist(double_plist *L)
{
      *L = (double_plist)malloc(sizeof(double_list));
      if(NULL == *L){
	    perror("malloc");
	    exit(1);
      }

      (*L)->next = *L;
      (*L)->prev= *L;
}
//h前插入new
void insert_doublelist(double_plist h,double_plist new)
{
      new->prev = h->prev;
      h->prev->next = new;
      new->next = h;
      h->prev = new;
}
//h后插入new
void insert_doublelist_post(double_plist h,double_plist new)
{
      new->next = h->next;
      h->next->prev = new;
      new->prev = h;
      h->next = new;
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
	    insert_doublelist(h,new);
	    show_doublelist(h);
      }
}

//判断是否为空
bool isempty_doublelist(double_plist h)
{
      if(h->prev == h->next)
	    return true;
      else
	    return false;
}

//删除p
void del_doublelist(double_plist p)
{
      p->next->prev = p->prev;
      p->prev->next = p->next;
}
//删除p前面的结点
void del_doublelist_pre(double_plist p)
{
      del_doublelist(p->prev);
}

//删除p后面的结点
void del_doublelist_post(double_plist p)
{
      del_doublelist(p->next);
}
void sort(double_plist h)
{
      double_plist p,q;
      p = h->prev;
      while(p != h){
	    if(p->data % 2 != 0){ //奇数，p向前移动
		  p = p->prev;
	    }else{//偶数，将p指向的结点剪切，插入到头结点前
		  q = p;
		  p = p->prev;

		  del_doublelist(q);
		  insert_doublelist(h,q);
		  show_doublelist(h);
	    }
      }
}
//遍历
void show_doublelist(double_plist h)
{
      double_plist p;

      for( p = h->next; p != h; p = p->next)
	    printf("%d\t",p->data);
      printf("\n");
}
