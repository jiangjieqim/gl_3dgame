#ifndef _TREE_H_
#define _TREE_H_

//构造一棵树
void* tree_new();

//为当前的树节点设置数据
void tree_set_data(void* t,void* data);

//获取树的节点数据
void* tree_get_data(void* t);

//为树节点添加子树节点,将data赋值到该子树节点的data字段
void tree_addchild(void* t,void* data);

//为树节点移除一个子节点
int tree_removeChild(void* t,void* node);

//遍历树节点
void tree_foreach(void* t,void (*callBack)(void*,void*),void* param);//treenode*,param*
//销毁树
void tree_dispose(void* p);
#endif
