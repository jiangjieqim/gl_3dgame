#ifndef _MAP_H_
#define _MAP_H_

struct MapNode{
	char* key;
	void* value;
};
//创建键值对象
void*
map_create();

//根据键值获取数据
void*
map_get(void* map,const char* key);
//删除一个键值
void
map_del_key(void*map,const char* key);
//设置一个键值对象值
void*
map_set(void* map,const char* key,void* value);
//销毁map值对象
void
map_dispose(void* map);

//根据键值对象中的值获取键值对象
void*
map_getNodeByValue(void* mapptr,void* value);

//打印输出map的节点列表信息
void
map_info(void* mapptr);

#endif