#include "btree.h"

int main(void)
{
      btree_pnode t;

      t = create_btree();   //创建二叉树
      
      travel("先序遍历序列为:\n",preorder,t);
      travel("先序非递归遍历序列为:\n",preorder_un,t);
      travel("中序遍历序列为:\n",midorder,t);
      travel("后序遍历序列为:\n",postorder,t);
      travel("层次遍历序列为:\n",levelorder,t);
      return 0;
}
