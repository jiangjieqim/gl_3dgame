::echo off
echo %time%开始编译
@rem 项目用vs2005编译

call initroot.bat

@rem ============================================================================================


@rem 增加本地include路径
set INCLUDE=%INCLUDE%%cd%\include\;%cd%\common\include\;%cd%\ftfont\;

set commonpath=%cd%\common\src

@rem 增加本地lib路径
set LIB=%LIB%%cd%\lib\;


set loaclfile=%cd%\dll\test.exe
if exist "%cd%\dll\test.exe" (del %loaclfile%) else (echo 不存在文件%loaclfile%)

cd src

echo _________ 编译%CD%\.*obj,编译%commonpath%\*.c _________

cl *.c %commonpath%\*.c /Ox

echo _________ 进行link.*obj _________
::pause

link -out:test.exe *.obj
echo %time%结束编译
del *.obj

copy test.exe ..\dll\test.exe
del *.exe 
cd ..\dll\ 

for /f "delims=" %%a in ('dir /b *.exe') do set /a "s+=%%~za/1024"
echo,%s% kb

test.exe 
 
cd ..

cd include

del *.exe

cd ..\src
del *.exe

::pause