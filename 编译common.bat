echo %time%
@rem 项目用vs2005编译

call initroot.bat

@rem ============================================================================================

@rem 增加本地include路径
set INCLUDE=%INCLUDE%%cd%\common\include\;


@rem 增加本地lib路径
::set LIB=%LIB%%cd%\lib\;

cd common\src\

cl -c *.c /O1
::cl  common.c array.c byte.c evt.c gettime.c list.c psv.c ring.c str.c tl_malloc.c tools.c tween.c vmath.c xml.c /O1

link -dll -out:common.dll *.obj

::打印导出的函数
dumpbin /exports common.dll  

copy common.dll ..\..\dll\common.dll 
copy common.lib ..\..\lib\common.lib 

del *.obj *.exp *.lib *.dll

pause 