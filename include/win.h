#ifndef _WIN_H_
#define _WIN_H_
/*包含windows.h相关接口*/
void win_alert(const char* title,const char* msg);/*alert弹出框*/
void win_useFontBitmaps(unsigned int* _textFont,int size);

int wl_LoadDll(const char* dll);
#endif