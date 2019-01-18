
//=============================================================================
#ifndef _PCSV_H_
#define _PCSV_H_
struct PCsvVO{
	/*
	数据行list
	*/
	struct LStackNode* list;

	/*
	关键字索引
	*/
	int keyIndex;

	/*
	引用数组,无需释放
	*/
	struct Array* keylist;
};

//测试用例
/************************************

struct PCsvVO * pcsv;
int out;
Array* arr;
tl_memState(TRUE);
printf("===========>%d bytes\n",tl_memByteSize());
pcsv = pcsv_load("D:\\source\\resource\\CaiLiaoHeCheng.csv","itemid",1);

arr=pcsv_getValue(pcsv,"qianghua10");
array_get(arr,1,&out);
if(out){
	printf("%s\n",(char*)out);
}
pcsv_dispose(pcsv);
printf("===========>%d bytes\n",tl_memByteSize());

***************************************/

/*
*	加载数据
*	const char* path	:加载的路径
*	const char* key		:主键
*	int checkIndex		:用第几行作为key名列表,从0开始(csv表中 只用一行作为主键键名.如果是第1行作为主键,checkIndex=0)
*/
struct PCsvVO* pcsv_load(const char* path,const char* mainkey,int checkIndex);
/*
销毁PCsvVO数据
*/
void pcsv_dispose(struct PCsvVO* p);

/*
根据keyValue匹配主键的值获取该行数据
注意:这里的Array不需要用array_free释放,因为在pcsv_dispose会释放
*/
struct Array* pcsv_getValue(struct PCsvVO* p,const char* keyValue);

//========================================================================
#endif