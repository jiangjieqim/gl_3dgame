//#define BUILDING_DLL
#include <string.h>
#include <stdio.h>
#include <memory.h>    
#include <stdlib.h>
#include <assert.h>
//#define DEBUG

#define BUILDING_DLL

//是否使用自定义的内存管理,此方式没有原生的malloc 和 free的性能更加好
#define CUST_MEMORY

#include "tools.h"
#include "gettime.h"
//*************************************
//日志相关的宏
//显示内存池中内存节点的真实使用大小
#define LOG_USE_SIZE
//显示内存节点的信息
#define LOG_NODE_DETAIL_INFO
//*************************************

#define EMemoryDisable  0//未使用
#define	EMemoryEnable   1//已经使用

struct LStackNode* memList;//内存节点列表
//	总字节长度
static int g_total;
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

	#ifdef LOG_USE_SIZE
	int realsize;
	#endif
};
struct MemFind{
	void* p;
	int size;
	int nodeLength;
	int curMin;//当前找到的最小的节点所占用的字节大小
};


//未使用的节点个数
//static int g_disable_cnt;

//未使用的节点数据字节总大小
//static int g_disable_bytes;
/************************************************************************/
/* 是否在memory_free之后直接free系统回收								*/
/************************************************************************/
static int g_bFreeClear = 1;

static int
f_getInfo(int data,int parm){
	struct MemInfo* info = (struct MemInfo*)parm;
	
	struct MemoryNode* node = (struct MemoryNode*)data;
	if(node->bUse == EMemoryDisable){
		//g_disable_cnt++;
		//g_disable_bytes+=node->length;

		if(info){
			info->disable_bytes += node->length;
			info->disable_cnt++;
		}

		#ifdef LOG_NODE_DETAIL_INFO
			log_color(0xffff00,"0x%x \t%d bytes \t未使用\n",node,node->length);
		#endif
	}
	else{
		#ifdef LOG_NODE_DETAIL_INFO
			int status = node->length!=node->realsize ? 1 : 0;
			log_color(0x00ff00,"0x%x \t%d bytes \t使用中\trealsize = %d\t%s\n",node,node->length,node->realsize,status==1?"复用":"");
		#endif
	}

	/*if(info){
		info->total+=node->length;
	}*/

	return 1;
}
/*
	节点信息计算
*/
static void
f_calculate_mem(struct MemInfo* info){
	//g_disable_bytes = 0;
	//g_disable_cnt = 0;
	
	if(memList){
		LStack_ergodic(memList,f_getInfo,(int)info);
	}
}

void memory_info(struct MemInfo* info){
	info->disable_bytes = 0;
	info->disable_cnt = 0;
	info->total = g_total;
	f_calculate_mem(info);
}
//
//void
//memory_get_info(int* pDisable_bytes,int* pDisable_cnt,int* pg_total){
//	f_calculate_mem(0);
//	//log_color(0xffff00,"空置节点总数:%d,总大小:%d字节,内存池总占用:%d字节\n",g_disable_cnt,g_disable_bytes,g_total);
//	//if(pDisable_cnt)*pDisable_cnt = g_disable_cnt;
//
//	//if(pDisable_bytes)*pDisable_bytes = g_disable_bytes;
//
//	if(pg_total)
//		*pg_total = g_total;
//}

static void
f_resizeByts(struct MemoryNode* node,int _size){
	void *p1,*p2;
	p2 = node->p;

	//重新对内存块进行整理,将多余的内存块交给系统回收
	p1 =realloc(node->p,_size);
	if(p1!=p2){
		log_color(0,"size=%d node->length = %d \n%0x\n%0x\n",_size,node->length,p1,p2);
		assert(0);
	}
	node->p = p1;
	node->length = _size;
}

static int
f_findnew(int data,int parm){
	struct MemFind* ptr = (struct MemFind*)parm;
	int _size = ptr->size;
	struct MemoryNode* node = (struct MemoryNode*)data;
	if(node->bUse == EMemoryDisable){
		//g_disable_cnt++;
		//g_disable_bytes+=node->length;
		if(node->length >= _size){
			
			if(ptr->curMin > node->length){
				ptr->curMin = node->length;
				ptr->p = node->p;
				ptr->nodeLength = node->length;
				
				f_resizeByts(node,_size);

				//memset(node->p,0,node->length);
				node->bUse = EMemoryEnable;
				
				#ifdef LOG_USE_SIZE
					node->realsize = _size;
				#endif

				//log_color(0xff0000,"%d 取内存池中的数据:0x%x  %d bytes \ttotal = %.3f kb\n",g_cnt,node->p,node->length,f_getTotalKB());
				//g_cnt++;
				return 1;
			}
		}
	}
	return 1;
}

static void*
memory_new(int size)
{
	struct MemoryNode* node;
	struct MemFind mf;
	if(!memList){
		memList =	LStack_create();
	}
	
	//多一个字节,处理\0情况,字符数串的结束标志
	//size += 1;

	//g_size = size;
	//g_node = 0;
	mf.p = 0;
	mf.size = size;
	mf.curMin  = 1024*1024;
	LStack_ergodic(memList,f_findnew,(int)&mf);

	if(mf.p){
		//log_color(0,"node len = %d\n",mf.nodeLength);
		return mf.p;
	}

	///////////////////////////////////////////////////
	node = (struct MemoryNode*)MALLOC(sizeof(struct MemoryNode));
	//memset(node,0,sizeof(struct MemoryNode));

	node->bUse = EMemoryEnable;
	node->length = size;
	g_total += size;
	node->p = MALLOC(size);

	#ifdef LOG_USE_SIZE
		node->realsize = size;
	#endif

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
	struct MemFind* ptr = (struct MemFind*)parm;
	struct MemoryNode* node = (struct MemoryNode*)data;
	if(node->p == ptr->p){
		node->bUse = EMemoryDisable;
		if(g_bFreeClear){
			f_freeNode(node);
		}
		return 0;
	}
	return 1;
}
struct GC_node{
	struct LStackNode* delList;
	struct LStackNode* useList;
};
static int 
f_gc(int data,int param){
	struct GC_node* gc = (struct GC_node*)param;

	struct LStackNode* delList = gc->delList;
	struct LStackNode* useList = gc->useList;
	struct MemoryNode* node = (struct MemoryNode*)data;
	if(node->bUse == EMemoryDisable){
		//f_freeNode(node);
		LStack_push((void*)delList,(void*)node);
	}else{
		LStack_push((void*)useList,(void*)node);
	}
	return 1;
}
//遍历删除
static int f_del(int data,int param){
	f_freeNode((struct MemoryNode*)data);
	return 1;
}

//Garbage Collection
void memory_gc(){
	int n = g_total;
	int m = get_longTime();
	if(memList){
		struct GC_node gcNode;
		gcNode.delList = LStack_create();
		gcNode.useList = LStack_create();

		//struct LStackNode* savelist = LStack_create();
		LStack_ergodic(memList,f_gc,(int)&gcNode);//遍历生成一个新的链表存储内存节点
		LStack_ergodic(gcNode.delList,f_del,0);
		log_color(0,"需要清理的节点数: = %d\n",LStack_length(gcNode.delList));

		log_color(0xffff00,"gc之前%d字节(%.3fkb),消耗%d毫秒,gc之后%d字节(%.3fkb),回收了%d字节(%.3fkb)\n",n,n/1024.0f,(get_longTime() - m), g_total,g_total / 1024.0f , (n - g_total), (float)(n - g_total) / 1024.0f);
		
		LStack_delete(gcNode.delList);
		LStack_delete(memList);
		memList = gcNode.useList;
	}else{
		printf("未使用自定义的内存管理\n");
	}
}
struct MemFind mf;
static void 
f_free(void *p,int _stat){
	
	g_bFreeClear = _stat;
	mf.p = p;
	//g_node = p;
	LStack_ergodic(memList,f_findfree,(int)&mf);
}

static void 
memory_retrieve(void* p){
	f_free(p,1);
}

static void  
memory_free(void* p){
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