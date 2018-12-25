#ifndef _MSOCKET_H_
#define _MSOCKET_H_


//extern "C" void _declspec(dllexport)msocket_send(char* ip,char* _data,int _dataLength,
//	char* pRecv,unsigned short _port,int recvByteSize); 


/*
	建立一个服务器监听
	======================================
	pRecvBuf:	用来接受数据
	isThread:	是否单独创建一个线程

*/
void msocket_server(unsigned short port,int pRecvBuflength,
	CallBackFun parseClient,int isThread);
/*
	向ip发送数据_data
*/
void msocket_send(char* ip,char* _data,int _dataLength,
	char* pRecv,unsigned short _port,int recvByteSize);

#endif