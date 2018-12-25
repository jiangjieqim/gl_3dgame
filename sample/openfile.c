/*********************************************************************************************************
 * 获取文件夹内所有文件名
 *********************************************************************************************************/

#include <stdio.h>
#include <io.h>
//negative positive
const char *to_search="D:\\gl_source\\sample\\*.c";    //欲查找的文件，支持通配符
//FILE *fp;

int main()
{
	long handle;                                                     //用于查找的句柄
	struct _finddata_t fileinfo;                                     //文件信息的结构体

	//fp = fopen("train_list2.txt", "w+");
	//if (fp==0) 
	/*{
		printf("can't open file\n");
		return 0;
	}*/
	handle=_findfirst(to_search,&fileinfo);                          //第一次查找
	if(-1==handle)
		return -1;

	//fprintf(fp,"positive/%s 0\r",fileinfo.name);                 //打印出找到的文件的文件名

	
	printf("%s\n",fileinfo.name);
	while(!_findnext(handle,&fileinfo))                              //循环查找其他符合的文件，直到找不到其他的为止
	{
		//fprintf(fp,"positive/%s 0\r",fileinfo.name);
		printf("%s\n",fileinfo.name);
	}
	_findclose(handle);                                              //关闭句柄
	
	printf("output done.\n");
	//fclose(fp);
	//system("pause");
	return 0;
}