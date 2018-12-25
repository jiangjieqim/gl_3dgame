@echo off
echo [当前要编译的文件名:(%cd%\%f%.c)]

@rem opengl按钮测试用例
@rem 一个小的界面框架程序,用于测试.
@rem 项目用vs2005编译

@rem 存储当前路径
set cur=%cd%

cd..

set p=dll\%f%.exe

IF exist %p% (del %p%) ELSE (echo [%p%文件不存在!])

call initroot.bat

@rem 增加本地include路径
set INCLUDE=%INCLUDE%%cd%\include\;%cur%\;

@rem 增加本地lib路径
set LIB=%LIB%%cd%\lib\;

cd include

@rem jgl.c gettime.c 
cl %cur%\%f%.c  tools.c gettime.c jgl.c

link *.obj
copy %f%.exe ..\dll\%f%.exe
del *.obj *.exe
..\dll\%f%.exe
pause