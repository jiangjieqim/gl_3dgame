echo off
echo %time%开始编译
@rem 项目用vs2005编译

call initroot.bat

@rem ============================================================================================


@rem 增加本地include路径
set INCLUDE=%INCLUDE%%cd%\include\;%cd%\common\include\;%cd%\lib_freetype2.9.1\include\;

@rem 增加本地lib路径
set LIB=%LIB%%cd%\lib\;%cd%\lib_freetype2.9.1\;


set loaclfile=%cd%\dll\test.exe
if exist "%cd%\dll\test.exe" (del %loaclfile%) else (echo 不存在文件%loaclfile%)

cd src

::cl test.c tlgl.c ex.c tmat.c md5.c jgl.c sprite.c atlas.c obj_vbo.c anim.c node.c collide.c animtor.c obj.c md2.c test_unit_01.c text.c bmp.c base.c frame.c /O1

cl *.c /Ox

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