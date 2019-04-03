#include <stdio.h>
//#include <list.h>
#include "list.h"

struct A{
    int a;
    float b;
};

int main(void)
{
    struct A st = {100,2.5};
    float *ptr = &st.b;

    printf("st.a = %d\n",st.a);
    
    printf("st.b = %f\n",st.b);
    printf("*ptr = %f\n",*ptr);
    //通过结构体中的成员b的地址，获取成员a
    struct A *p;
     p = list_entry(ptr,struct A,b);
     printf("p->a = %d\n",p->a);
    return 0;
}

