#include "linkstack.h"

void init_linkstack(link_pstack * Top)
{
      *Top = NULL;
}

void push_linkstack(link_pstack *Top,datatype d)
{
      link_pstack new;
      new = (link_pstack)malloc(sizeof(link_stack));
      if(NULL == new){
	    perror("malloc");
	    exit(1);
      }
      new->data = d;
      
      new->next = *Top;
      *Top = new;
}
void pop_linkstack(link_pstack *Top,datatype *d)
{
      link_pstack t;
      if(isempty_linkstack(*Top)){
	    printf("栈已空!\n");
	    return ;
      }

      t = *Top;
      *d = t->data;
      *Top = (*Top)->next;
      free(t);

}

bool isempty_linkstack(link_pstack top )
{
      if(top == NULL)
	    return true;
      else
	    return false;
}
#if 0
void show_linkstack(link_pstack top)
{
      for(;top != NULL; top = top->next)
	    printf("%d\t",top->data);
      printf("\n");
}
#endif

