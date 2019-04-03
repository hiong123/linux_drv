#include "linkqueue.h"
#if 0
void init_linkqueue(link_pqueue * Q)
{
      *Q = (link_pqueue)malloc(sizeof(link_queue));
      if(NULL == *Q){
	    perror("malloc");
	    exit(1);
      }
      (*Q)->front = (*Q)->rear = SIZE-1;
}
#else
link_pqueue init_linkqueue(void)
{
      link_pqueue q;
      q = (link_pqueue)malloc(sizeof(link_queue));
      if(NULL == q){
	    perror("malloc");
	    exit(1);
      }
      q->front = (list_pnode)malloc(sizeof(list_node));
      if(NULL == q->front){
	    perror("malloc");
	    exit(1);
      }
      q->front->next = NULL;
      q->rear = q->front;
      return q;
}
#endif
bool in_linkqueue(link_pqueue q,datatype d)
{
     list_pnode new;

      new = (list_pnode)malloc(sizeof(list_node));
      if(NULL == new){
	    perror("malloc");
	    return false;
      }
       new->data = d;
       //new指向的结点入队
       new->next = q->rear->next;
       q->rear->next = new;
       q->rear = q->rear->next;
      return true;
}
bool out_linkqueue(link_pqueue q,datatype *d)
{
      list_pnode t;
      if(isempty_linkqueue(q)){
	    printf("栈已空!\n");
	    return false;
      }

      t = q->front;
      q->front = q->front->next;
      *d = q->front->data;
      free(t);
      return true;
}

bool isempty_linkqueue(link_pqueue q)
{
      if(q->front == q->rear)
	    return true;
      else
	    return false;
}
void show_linkqueue(link_pqueue q)
{
      list_pnode p;
      for(p = q->front->next; p != NULL ; p = p->next)
	    printf("%d\t",p->data);
      printf("\n");
}


