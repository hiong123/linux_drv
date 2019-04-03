#include "seqstack.h"

void init_seqstack(seq_pstack * S)
{
      *S = (seq_pstack)malloc(sizeof(seq_stack));
      if(NULL == *S){
	    perror("malloc");
	    exit(1);
      }
      (*S)->top = -1;
}

void push_seqstack(seq_pstack s,datatype d)
{
      if(isfull_seqstack(s)){
	    printf("栈已满!\n");
	    return ;
      }

      s->top++;
      s->data[s->top] = d;
}
void pop_seqstack(seq_pstack s,datatype *d)
{
      if(isempty_seqstack(s)){
	    printf("栈已空!\n");
	    return ;
      }
      *d = s->data[s->top];
      s->top--;
}

bool isfull_seqstack(seq_pstack s)
{
      if(s->top == SIZE-1)
	    return true;
      else
	    return false;
}
bool isempty_seqstack(seq_pstack s)
{
      if(s->top == -1)
	    return true;
      else
	    return false;
}
void show_seqstack(seq_pstack s)
{
      int i;
      for(i = 0; i <= s->top; i++)
	    printf("%d\t",s->data[i]);
      printf("\n");
}


