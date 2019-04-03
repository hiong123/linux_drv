#include "linklist.h"

void linksort(link_plist h);
int main(void)
{
      link_plist h;

      init_linklist(&h);
      create(h);
      linksort(h);
      return 0;
}

void linksort(link_plist h)
{
      link_plist p,q,t1,t2;

      p = h->next;
      h->next = NULL;
      while(p != NULL){
	    q = p;
	    p = p->next;
#if 0	    
	    for(t1 = h,t2 = h->next;t2!=NULL&&q->data > t2->data;t1 = t2,t2 = t2->next);
#else
	    t1 = h;
	    t2 = h->next;
	    while(t2 != NULL && q->data > t2->data){
		  t1 = t2;
		  t2 = t2->next;
	    }
#endif
	    q->next = t2;
	    t1->next = q;

	    show_linklist(h);
      }
}
