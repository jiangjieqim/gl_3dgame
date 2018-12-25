

/************************************************************************/
/*      内存池                                                  */
/************************************************************************/
#define MEM_USED 0//已经使用
#define MEM_UN_USED 1//没有使用

#define MEM_IS_FOUND 0		//找到
#define MEM_NOT_FOUNT -1	//找不到匹配的尺寸
#define MEM_NEED_MORE -2	//需要更大的size

struct MemNode{
	char* p;
	int len;
	char uesd;
};

struct FNode{
	//需要的尺寸
	int needSize;

	//返回结果
	struct MemNode* out;
};

//struct MemCutNode{
//	int len;
//};

struct MemHandle{
	struct LStackNode* list;	
	int maxSize;
	int pos;
	void* buffer;
	char pool[G_BUFFER_256_SIZE];

	int bufferList[G_BUFFER_64_SIZE];
	int bufferCount;

	/*
	内存池开关状态(TRUE,FALSE)
	*/
	int stat;
};

//#define struct MemHandle struct MemHandle
#define N struct MemNode

struct MemHandle* _memInstance;

//内存池句柄
static struct MemHandle* GetInstance(){
	if(_memInstance==NULL)
	{
		printf("内存池未初始化!!!\n");
		assert(0);
	}
	return _memInstance;
}

static struct MemNode* addNode(struct MemHandle * p, int len){
	struct MemNode * m = NULL;
	if(p->pos+len > p->maxSize){
		printf("大于最大内存尺寸\n");
		return NULL;
	}
	m = (struct MemNode * )MALLOC(sizeof(struct MemNode));
	m->len = len;
	m->uesd = MEM_UN_USED;
	m->p = (char*)p->buffer + p->pos + len;

	//printf("pos:%d,len:%d字节,内存块地址 0x%x\n",p->pos,len,m->p);

	LStack_push((void*)p->list,(int)m);
	p->pos+=len;   
	return m;
}

static int findNodeCallBack(int data,int parm){
	struct MemNode* n = (struct MemNode*)data;
	struct FNode* _pNode = (struct FNode*)parm;
	if(n->uesd == MEM_UN_USED && n->len == _pNode->needSize){
		_pNode->out = n;
	}
	return 1;
}

/*
*
*	搜索可以使用的内存节点
*
*/
static struct MemNode* findNode(struct MemHandle* p,int len){
 	struct FNode _node;
	memset(&_node,0,sizeof(struct FNode));
	
	//需要的字节长度
	_node.needSize = len;

	LStack_ergodic(p->list,findNodeCallBack,(int)&_node);

	//搜索到的字节结点
	return _node.out;
}

static int memSizeCallBack(int data,int parm){
	int *p = (int*)parm;
	struct MemNode* _node = (struct MemNode*)data;
	if(_node->uesd == MEM_USED){
		*p +=_node->len;
	}
	return 1;
}

int tl_memByteSize(){
	if(GetInstance()->stat){
		int size = 0;
		LStack_ergodic(GetInstance()->list,memSizeCallBack,(int)&size);
		return size;
	}else{
		//printf("未使用内存池模式,取出来的数据有问题\n");
	}
	return -1;
}

int Split(char* p){
	int* parms = (int*)p;
	int i;
	struct MemHandle * instance=(struct MemHandle *)parms[0];
	char* str = (char*)parms[2];
	int buffer,bufferSize;
	sscanf(str,"%d,%d",&buffer,&bufferSize);

	for(i = 0;i < bufferSize;i++){
		addNode(instance,buffer);//生成bufferSize个相同尺寸的内存块节点
	}
	return 1;
}

int SplitAdd(char* p){
	int* parms = (int*)p;
	struct MemHandle * _instance=(struct MemHandle *)parms[0];
	char* str = (char*)parms[2];
	int buffer,bufferSize;
	sscanf(str,"%d,%d",&buffer,&bufferSize);
	_instance->maxSize+=bufferSize * buffer;
	_instance->bufferList[_instance->bufferCount] = buffer;
	_instance->bufferCount++;
	printf("%d ",buffer);
	return 1;
}
/*
获取需要的缓存大小
*/
void GetNeedSize(struct MemHandle * p,char* pool)
{
	int parms[3];
	p->bufferCount = 0;
	p->maxSize = 0;
	parms[0] = (int)p;
	parms[1] = 0;
	//获取需要的缓存大小
	tl_split_s((const char*)pool,G_SIGN_AITA,SplitAdd,parms,2);
}

void tl_memInit(char* pool){
	int i = 0;
	int parms[3];

	struct MemHandle * p = (struct MemHandle *)MALLOC(sizeof(struct MemHandle));
	_memInstance = p;

	p->maxSize = 0;

	GetNeedSize(p,pool);
	p->pos = 0;
	p->list = LStack_create();
	p->buffer = (void*)MALLOC(p->maxSize);

	parms[0] = (int)p;
	parms[1] = 0;
	tl_split_s((const char*)pool,G_SIGN_AITA,Split,parms,2);
	tl_bubbleSortInt(p->bufferList,p->bufferCount);
	printf("内存池总大小%f mb,内存池已分配大小:%f mb\n",(float)(p->maxSize)/1024/1024,(float)p->pos/1024/1024);
}

/*
返回-1,表示未找到可使用的尺寸

return		-2	需要更大的size
return		-1	找不到匹配的尺寸

*/
static int getNextSize(int size,int* list,int len,int* outSize){
	int i;
	int _stat=MEM_NOT_FOUNT;
	//tl_bubbleSortInt(list,len);
	if(len > 0 && size < list[0]){
		_stat = MEM_IS_FOUND;
		*outSize = list[0];
	}
	else if(len > 0 && size > list[len-1]){
		//需要一个更大的内存块
		_stat = MEM_NEED_MORE;
	}else{
		for(i = 0;i<len-1;i++){
			int _l = list[i];
			int _r = list[i+1];
			if(size >= _l && size < _r ){
				_stat = MEM_IS_FOUND;
				*outSize = _r;
				break;;
			}
		}
	}
	return _stat;
}

void* tl_malloc(int size){
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
			//instance->maxSize+=fSize;
			//realloc(instance->buffer,instance->maxSize);
			//struct MemNode* _n=	addNode(instance ,fSize);
			//return _n->p;
			return tl_malloc(fSize);
		}
		printf("tl_malloc (%d bytes) fail!\n",size);
		return NULL;
	}
	//printf("MALLOC ");
	return MALLOC(size);

}
int CallBackFindDel(char* parms){
	int* point = (int*)parms;
	N* n = (N*)point[0];
	char* target = (char*)point[1];
	if(n->p ==target && n->uesd == MEM_USED){
		point[3] = TRUE;
		memset(n->p,0,n->len);//可以注释,性能较好
		n->uesd = MEM_UN_USED;
		point[2] = (int)n;
	}
	return 0;
}

struct MemDelNode{
	void* target;//	target
	struct MemNode* result;//储存结果
	int stat;//	resulg is OK	是否删除成功 TL_TRUE:成功 TL_FALSE:失败
};

/*
	删除内存结点
*/
static int freeDelNode(int data,int parm){
	struct MemNode* n = (struct MemNode*)data;
	struct MemDelNode* delNode = (struct MemDelNode*)parm;
	char* target =	(char*)delNode->target;
	if(n->p ==target && n->uesd == MEM_USED){
		delNode->stat = TRUE;
		memset(n->p,0,n->len);//可以注释,性能较好
		n->uesd = MEM_UN_USED;
		delNode->result = n;
	}
	return 1;
}
void tl_free(void* p){
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
}
//删除内存池
void tl_memdel(){
	struct MemHandle* t = GetInstance();
	FREE(t->buffer);
	t->buffer = NULL;
	FREE(t);
	t = NULL;
}

void tl_memState(int _stat){
	struct MemHandle* t = GetInstance();
	t->stat=_stat;
}
