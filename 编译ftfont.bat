echo %time%
@rem 项目用vs2005编译

call initroot.bat

@rem ============================================================================================

@rem 增加本地include路径
set INCLUDE=%INCLUDE%%cd%\lib_freetype2.9.1\include\;%cd%\common\include\


@rem 增加本地lib路径
set LIB=%LIB%%cd%\lib_freetype2.9.1\;

cd ftfont

cl -c *.c /Ox


link -dll -out:ftfont.dll *.obj

::打印导出的函数
dumpbin /exports ftfont.dll  

copy ftfont.dll ..\dll\ftfont.dll 
copy ftfont.dll ..\Test\Debug\ftfont.dll 
copy ftfont.lib ..\lib\ftfont.lib 


::@echo off
::提取txt、doc、xls文件总大小
for /f "delims=" %%a in ('dir /b *.dll') do set /a "s+=%%~za/1024"
echo,%s% kb

del *.obj *.exp *.lib *.dll

::pause 