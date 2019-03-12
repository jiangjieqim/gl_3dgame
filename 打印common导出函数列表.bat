call initroot.bat
cd dll
dumpbin /exports common.dll 
for /f "delims=" %%a in ('dir /b common.dll') do set /a "s+=%%~za/1024"
echo,common.dll,%s% kb
pause