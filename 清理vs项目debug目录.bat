cd dll
del *.exe
cd..

cd Test/

for /r %%i in (debug,release,ipch) do rd /s /q "%%i"
for /r %%i in (*.sdf) do del /s /f /q "%%i"
del *.ncb *.user

cd..
cd common

for /r %%i in (debug,release,ipch) do rd /s /q "%%i"
for /r %%i in (*.sdf) do del /s /f /q "%%i"
del *.ncb *.user

cd..
cd ftfont
for /r %%i in (debug,release,ipch) do rd /s /q "%%i"
for /r %%i in (*.sdf) do del /s /f /q "%%i"
del *.ncb *.user

cd..
cd dll
del common.dll
del *.exe

cd..
cd lib
del common.lib

echo %CD%
cd../dll
del common.dll ftfont.dll

cd../lib
del common.lib ftfont.lib

::pause