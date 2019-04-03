/*
 2、用单向链表实现数据逆转，首先建立一个包含若干整数的单向链表，然后参考讲义的例子实现数据的逆转。（比如说把1，2，3，4，5逆转成5，4，3，2，1）
 */
#include "linklist.h"

int main(void)
{
      link_plist head;
      datatype data;

      //初始化空链表
      init_linklist(&head);
      //建立包含若干整数的单向链表
      create(head);
      //逆转
      sort(head);
      printf("请输入要删除的整数:");
      scanf("%d",&data);
      del(head,data);
      show_linklist(head);
      return 0;
}
