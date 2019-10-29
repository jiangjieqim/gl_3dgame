//#define BUILDING_DLL
#include <string.h>
#include <stdio.h>
#include <memory.h>    
#include <stdlib.h>

//#define DEBUG

#define BUILDING_DLL

//是否使用自定义的内存管理,此方式没有原生的malloc 和 free的性能更加好
#define CUST_MEMORY

#include "tools.h"
#include "gettime.h"

#define EMemoryDisable  0//未使用
#define	EMemoryEnable   1//已经使用

struct LStackNode* memList;//内存节点列表

struct MemoryNode{
	/**
	* 字节长度
	*/
	void* p;
	/**
	* 字节长度                                                                  
	*/
	int length;
	/**
	* 是否使用了                                                                    
	*/
	int bUse;
};

//	总字节长度
static int g_total;

//存储需要申请的内存块大小
static int g_size;

//临时节点
static void* g_node;

//未使用的节点个数
static int g_disable_cnt;

//未使用的节点数据字节总大小
static int g_disable_bytes;
/************************************************************************/
/* 是否在memory_free之后直接free系统回收                         */
/************************************************************************/
static int g_bFreeClear = 1;

static int
f_getInfo(int data,int parm){
	struct MemoryNode* node = (struct MemoryNode*)data;
	if(node->bUse == EMemoryDisable){
		g_disable_cnt++;
		g_disable_bytes+=node->length;
		//log_color(0xffff00,"0x%x \t%d bytes\n",node,node->length);
	}
	else{
		//log_color(0x00ff00,"0x%x \t%d bytes\n",node,node->length);
	}
	return 1;
}
/*
	节点信息计算
*/
static void
f_calculate_mem(){
	g_disable_bytes = 0;
	g_disable_cnt = 0;
	if(memList){
		LStack_ergodic(memList,f_getInfo,0);
	}
}
void
memory_get_info(int* pDisable_bytes,int* pDisable_cnt,int* pg_total){
	f_calculate_mem();
	//log_color(0xffff00,"空置节点总数:%d,总大小:%d字节,内存池总占用:%d字节\n",g_disable_cnt,g_disable_bytes,g_total);
	if(pDisable_cnt)*pDisable_cnt = g_disable_cnt;

	if(pDisable_bytes)*pDisable_bytes = g_disable_bytes;

	if(pg_total)
		*pg_total = g_total;
}

static int
f_findnew(int data,int parm)
{
	struct MemoryNode* node = (struct MemoryNode*)data;
	if(node->bUse == EMemoryDisable){
		g_disable_cnt++;
		g_disable_bytes+=node->length;
		if(node->length >= g_size){
			g_node = node->p;
			//memset(node->p,0,node->length);
			node->bUse = EMemoryEnable;
			//log_color(0xff0000,"%d 取内存池中的数据:0x%x  %d bytes \ttotal = %.3f kb\n",g_cnt,node->p,node->length,f_getTotalKB());
			//g_cnt++;
			return 0;
		}
	}
	return 1;
}

static void*
memory_new(int size)
{
	struct MemoryNode* node;
	if(!memList){
		memList =	LStack_create();
	}
	
	//多一个字节,处理\0情况,字符数串的结束标志
	size += 1;

	g_size = size;
	g_node = 0;

	LStack_ergodic(memList,f_findnew,0);


	if(g_node){
		return g_node;
	}

	///////////////////////////////////////////////////
	node = (struct MemoryNode*)MALLOC(sizeof(struct MemoryNode));
	//memset(node,0,sizeof(struct MemoryNode));

	node->bUse = EMemoryEnable;
	node->length = size;
	g_total += size;
	node->p = MALLOC(size);
	memset(node->p,0,size);

	LStack_push(memList,node);

	//log_color(0xff0000,"memory_new 内存申请开辟 %.3f kb\n",(float)(g_total)/1024);

	return node->p;
}

//删除一个节点会调用free系统回收内存,会增加系统GC负担
static void
f_freeNode(struct MemoryNode* node){
	g_total-=node->length;
	FREE(node->p);
	LStack_delNode(memList,(int)node);
}

//遍历删除
static int 
f_findfree(int data,int parm){
	struct MemoryNode* node = (struct MemoryNode*)data;
	if(node->p == g_node){
		node->bUse = EMemoryDisable;
		if(g_bFreeClear){
			f_freeNode(node);
		}
		return 0;
	}
	return 1;
}

//遍历删除
static int 
f_gc(int data,int parm){
	struct MemoryNode* node = (struct MemoryNode*)data;
	if(node->bUse == EMemoryDisable){
		f_freeNode(node);
	}
	return 1;
}

/*
int memory_get_total(){
	return g_total;
}
*/

//Garbage Collection
void memory_gc(){
	int n = g_total;
	int m = get_longTime();
	if(memList){
		LStack_ergodic(memList,f_gc,0);
		log_color(0xffff00,"gc之前%d字节(%.3fkb),消耗%d毫秒,gc之后%d字节(%.3fkb),回收了%d字节(%.3fkb)\n",n,n/1024.0f,(get_longTime() - m), g_total,g_total / 1024.0f , (n - g_total), (float)(n - g_total) / 1024.0f);
	}else{
		printf("未使用自定义的内存管理\n");
	}
}

static void 
f_free(void *p,int _stat){
	g_bFreeClear = _stat;
	g_node = p;
	LStack_ergodic(memList,f_findfree,0);
}

static void 
memory_retrieve(void* p){
	f_free(p,1);
}

static void  
memory_free(void* p){
/*
	int stat;
	va_list ap; 
	va_start(ap, p);
	stat = va_arg(ap, int);
	g_bFreeClear = stat == TRUE ? TRUE : FALSE;
	log_color(0xff0000,"%d %d\n",stat,g_bFreeClear);
*/
	f_free(p,0);
}

//static int m;
void* tl_malloc(int size){
#ifdef CUST_MEMORY
	void* p = memory_new(size);
#else
	void*p =malloc(size);
	if(p == 0){
		printf("tl_malloc size %d false",size);
	}
#endif
	//m++;
#ifdef DEBUG
	printf("+ \t%0x\n",p);
#endif
	return p;
/*

	struct MemHandle* instance = GetInstance();
	if(instance->stat){
		int needSize;	
		//此方法可以使用配置生成

		int fSize = 0;
		N* _node = NULL;
		int _stat= getNextSize(size,instance->bufferList,instance->bufferCount,&needSize);

		if(_stat==MEM_NOT_FOUNT)
		{
			printf("请求的内存为:%d字节,重新设置内存池大小\n",size);
			assert(0);
		}
		else if(_stat == MEM_NEED_MORE)
		{
			printf("需要更大的内存块 %d\n",size);
			assert(0);
		}

		fSize = needSize;

		_node=findNode(instance,fSize);
		if(_node != NULL){
			_node->uesd = MEM_USED;
#ifdef _SHOW_MALLOC_LOG_
			printf("申请 0x%x %d/%d\n",n,_node->len,tl_memsize());
#endif
			return _node->p;
		}else{
			//内存池内存不够,为内存池添加节点
			
			//printf("请求内存%d字节未找到,可以重新申请的内存块,检索内存:%d字节\n",size,fSize);
			
			
			//assert(0);
		
		
		//printf("新申请内存:%d字节\n",fSize);
		//	instance->maxSize+=fSize;
		//	realloc(instance->buffer,instance->maxSize);
		//	{
		//		struct MemNode* _n=	ex_addNode(instance ,fSize);
		//		return _n->p;
		//	}
			
			
			return tl_malloc(fSize);
		}
		printf("tl_malloc (%d bytes) fail!\n",size);
		return NULL;
	}
	//printf("MALLOC ");
	return MALLOC(size);
*/
}
//static int c;
void tl_free(void* p){
	#ifdef CUST_MEMORY
	memory_free(p);
#else
	free(p);
#endif
#ifdef DEBUG
	printf("\tfree %0x\n",p); 
	//c++;
#endif
	return;
/*
	struct MemHandle* t = GetInstance();
	if(t->stat){	

		struct MemDelNode delNode;
		memset(&delNode,0,sizeof(struct MemDelNode));
		delNode.stat = FALSE;
		delNode.target = p;
		LStack_ergodic(GetInstance()->list,freeDelNode,(int)&delNode);

		if(delNode.stat== FALSE){
			printf("tl_free 0x%x 失败,找不到释放的节点!\n",(int)p);
			assert(0);
		}else{
			struct MemNode* n = delNode.result;
			#ifdef _SHOW_MALLOC_LOG_
			printf("        释放 0x%x %d/%d\n",n,n->len,tl_memsize());
			#endif
		}
		p = NULL;

		return;
	}
	FREE(p);
	*/
}