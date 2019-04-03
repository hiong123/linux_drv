#include "linklist.h"

//初始化
void init_linklist(link_plist *L)
{
      *L = (link_plist)malloc(sizeof(link_list));
      if(NULL == *L){
	    perror("malloc");
	    exit(1);
      }

      (*L)->next = NULL;
}

void insert_linklist(link_plist p,link_plist new)
{
      new->next = p->next;
      p->next = new;
}

void create(link_plist h)
{
      int n,i;
      link_plist new,p = h;
      printf("请输入整数的个数:");
      scanf("%d",&n);
      for(i = 0;i < n; i++){
	    new= (link_plist)malloc(sizeof(link_list));
	    if(NULL == new){
		  perror("malloc");
		  exit(1);
	    }
	    printf("请输入一个整数:");
	    scanf("%d",&new->data);
	    insert_linklist(p,new);
	    p = p->next;  //让p指向表尾
	    show_linklist(h);
      }
}

//判断是否为空
bool isempty_linklist(link_plist h)
{
      if(h->next == NULL)
	    return true;
      else
	    return false;
}

void del_linklist(link_plist p)
{
      link_plist t;
      t = p->next;
      p->next = t->next;
      free(t);
}
void del(link_plist h,datatype d)
{
      link_plist p1,p2;
      if(isempty_linklist(h)){
	    printf("链表为空!\n");
	    return ;
      }

      for( p1 = h,p2 = h->next; p2 != NULL ; p1 = p2,p2 = p2->next)
	    if(p2->data == d)
		  del_linklist(p1);
}
void sort(link_plist h)
{
      link_plist t,p;

      p = h->next;
      h->next = NULL;
      while(p != NULL){
	    t = p;
	    p = p->next;
	    
	    insert_linklist(h,t);
	    show_linklist(h);
      }
}
//遍历
void show_linklist(link_plist h)
{
      link_plist p;

      for( p = h->next; p != NULL; p = p->next)
	    printf("%d\t",p->data);
      printf("\n");
}
