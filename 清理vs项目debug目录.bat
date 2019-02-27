cd dll
del *.exe
cd..

cd Test/

for /r %%i in (debug,ipch) do rd /s /q "%%i"
for /r %%i in (*.sdf) do del /s /f /q "%%i"
del *.ncb *.user

cd..
cd common

for /r %%i in (debug,ipch) do rd /s /q "%%i"
for /r %%i in (*.sdf) do del /s /f /q "%%i"
del *.ncb *.user


cd..
cd dll
del common.dll

cd..
cd lib
del common.lib

