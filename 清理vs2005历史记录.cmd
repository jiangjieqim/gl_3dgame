@echo off

@REG Delete HKCU\Software\Microsoft\VisualStudio\8.0\FileMRUList /va /f

@REG Delete HKCU\Software\Microsoft\VisualStudio\8.0\ProjectMRUList /va /f
pause