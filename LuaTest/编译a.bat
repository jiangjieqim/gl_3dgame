cd ..
call initroot.bat

cd LuaTest

@rem 增加本地include路径
set INCLUDE=%INCLUDE%%cd%\include\;

@rem 增加本地lib路径
set LIB=%LIB%%cd%\lib\;

cd out
del *.exe

cd ..

cl a.c
del *obj

copy a.exe out\a.exe
del *exe

cd out
a.exe

pause


