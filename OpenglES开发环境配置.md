1.无法启动丢失LibMaliEmulator.dll,log4cplus.dll


2.异常
FATAL -GLES:(GLNativeProxy::FunctionTable::GLFunction(void __cdecl(unsigned int,int,int)>::checkMissingVersions 2455
 ub 'opengl32.dll' (require for version of GL)!


3.crt0dat.c 的__crtExitProcess ExitProcess(status);  
es_t.exe 中的 0x778022e2 处未处理的异常: 0xC0000005: 写入位置 0x00000004 时发生访问冲突  
```
void __cdecl __crtExitProcess (
        int status
        )
{
        __crtCorExitProcess(status);

        /*
         * Either mscoree.dll isn't loaded,
         * or CorExitProcess isn't exported from mscoree.dll,
         * or CorExitProcess returned (should never happen).
         * Just call ExitProcess.
         */

        ExitProcess(status);
}
```
在 ExitProcess方法这里异常Crash  

4.环境配置  
依赖的dll:  
openglessl\Mali-T600_r7p0-00rel0.dll  
libEGL.dll,libGLESv2.dll,libMaliEmulator.dll,log4cplus.dll  


下载Mali_OpenGL_ES_Emulator-v3.0.2.g694a9-Windows-32bit.zip  
模拟器下载地址:
https://developer.arm.com/tools-and-software/graphics-and-gaming/opengl-es-emulator/downloads  



实例代码下载地址:  
git clone http://www.github.com/danginsburg/opengles3-book  

***src  
...\opengles3-book-master\Common\Source  
esShader.c,esShapes.c,esTransform.c,esUtil.c  

...\opengles3-book-master\Common\Source\Win32  
esUtil_win32.c  

***include  
...\opengles3-book-master\Common\Include  
esUtil.h,esUtil_win.h  

Mali_OpenGL_ES_Emulator-v3.0.2.g694a9-Windows-32bit\include  

***lib  
...\Mali_OpenGL_ES_Emulator-v3.0.2.g694a9-Windows-32bit\
libGLESv2.lib,libEGL.lib  

>>>>>>>>>>>>>>>>>>>>>>编译脚本>>>>>>>>>>>>>>>>>>>>  

环境变量初始化initroot.bat  
```
@rem 项目用vs2005编译
@echo off
IF exist "c:\Program Files (x86)" (set v=0) ELSE (set v=1)
set ROOT=Program Files (x86)
IF %v%==1 (set ROOT=Program Files)
set PATH=C:\%ROOT%\Microsoft Visual Studio 8\VC\bin;C:\%ROOT%\Microsoft Visual Studio 8\Common7\IDE;
set INCLUDE=C:\%ROOT%\Microsoft Visual Studio 8\VC\include;C:\%ROOT%\Microsoft Visual Studio 8\VC\PlatformSDK\Include;
set LIB=C:\%ROOT%\Microsoft Visual Studio 8\VC\lib\;C:\%ROOT%\Microsoft Visual Studio 8\VC\PlatformSDK\Lib;
```

esUtil_win32.obj : error LNK2019: 无法解析的外部符号 __imp__GetCursorPos@4，该符号在函数 _ESWindowProc@16 中被引用  
解决方案:增加  #pragma comment(lib,"user32.lib")  

esUtil_win32.obj : error LNK2019: 无法解析的外部符号 __imp__GetStockObject@4，该符号在函数 _WinCreate 中被引用
 解决方案:增加  #pragma comment(lib,"gdi32.lib")  

在入口文件最上面加上
```
#pragma comment (lib,"libEGL.lib")
#pragma comment (lib,"libGLESv2")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
```