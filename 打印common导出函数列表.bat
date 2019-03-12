call initroot.bat
cd dll

set fileName=common.dll
dumpbin /exports %fileName%
for /f "delims=" %%a in ('dir /b %fileName%') do set /a "s+=%%~za/1024"
echo,%fileName%,%s% kb
pause