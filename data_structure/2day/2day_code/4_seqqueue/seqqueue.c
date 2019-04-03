#include "seqqueue.h"

void init_seqqueue(seq_pqueue * Q)
{
      *Q = (seq_pqueue)malloc(sizeof(seq_queue));
      if(NULL == *Q){
	    perror("malloc");
	    exit(1);
      }
      (*Q)->front = (*Q)->rear = SIZE-1;
}

bool in_seqqueue(seq_pqueue q,datatype d)
{
      if(isfull_seqqueue(q)){
	    printf("栈已满!\n");
	    return false;
      }
      
      q->rear = (q->rear + 1) % SIZE;
      q->data[q->rear] = d;
      return true;
}
bool out_seqqueue(seq_pqueue q,datatype *d)
{
      if(isempty_seqqueue(q)){
	    printf("栈已空!\n");
	    return false;
      }
      q->front = (q->front+1) % SIZE;
      *d = q->data[q->front];
      return true;
}

bool isfull_seqqueue(seq_pqueue q)
{
      if((q->rear+1) %SIZE == q->front)
	    return true;
      else
	    return false;
}
bool isempty_seqqueue(seq_pqueue q)
{
      if(q->front == q->rear)
	    return true;
      else
	    return false;
}
void show_seqqueue(seq_pqueue q)
{
      int i;
      for(i = (q->front+1)%SIZE;i != (q->rear+1)%SIZE ; i = (i+1)%SIZE)
	    printf("%d\t",q->data[i]);
      printf("\n");
}


