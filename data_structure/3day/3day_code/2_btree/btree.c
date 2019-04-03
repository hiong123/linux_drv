#include "btree.h"
#include "linkqueue.h"
#include "linkstack.h"


btree_pnode create_btree(void)
{
      datatype_bt ch;
      btree_pnode t;

      scanf("%c",&ch);
      if(ch == '#')
	    t = NULL;
      else{
	    //创建根结点
	    t = (btree_pnode)malloc(sizeof(btree_node));
	    if(NULL == t){
		  perror("malloc");
		  exit(1);
	    }
	    t->data = ch;

	    t->lchild = create_btree();  //创建左子树
	    t->rchild = create_btree();  //创建右子树
      }
      return t;
}

//先序遍历
void preorder(btree_pnode t)
{
      if(t != NULL){
	    printf("%c",t->data);  //访问根结点
	    //先序遍历左子树
	    preorder(t->lchild);
	    //先序遍历右子树
	    preorder(t->rchild);
      }
}
//先序非递归
void preorder_un(btree_pnode t)
{
      link_pstack top;

      init_linkstack(&top);
      while(t != NULL || !isempty_linkstack(top)){
	    if(t != NULL){
		  printf("%c",t->data);
		  if(t->rchild != NULL)
			push_linkstack(&top,t->rchild);
		  t = t->lchild;
	    }else
		  pop_linkstack(&top,&t);
      }
}
//中序遍历
void midorder(btree_pnode t)
{
      if(t != NULL){
	    //中序遍历左子树
	    midorder(t->lchild);
	    printf("%c",t->data);  //访问根结点
	    //中序遍历右子树
	    midorder(t->rchild);
      }
}
//后序遍历
void postorder(btree_pnode t)
{
      if(t != NULL){
	    //先序遍历左子树
	    postorder(t->lchild);
	    //先序遍历右子树
	    postorder(t->rchild);
	    printf("%c",t->data);  //访问根结点
      }
}
//层次遍历
void levelorder(btree_pnode t)
{
      link_pqueue q;

      q = init_linkqueue();  //初始化链式队列
      while(t != NULL){
	    printf("%c",t->data);
	    if(t->lchild != NULL)
		  in_linkqueue(q,t->lchild);
	    if(t->rchild != NULL)
		  in_linkqueue(q,t->rchild);
	    if(isempty_linkqueue(q))
		  break;
	    else
		  out_linkqueue(q,&t);
      }
}

void travel(char const *str,void (*porder)(btree_pnode),btree_pnode t)
{
      printf("%s",str);
      porder(t);
      printf("\n");
}
