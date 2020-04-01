## 删除相关项
删除最近打开的历史记录  
删除最近打开的项目列表  
```
@echo off
@REG Delete HKCU\Software\Microsoft\VisualStudio\8.0\FileMRUList /va /f
@REG Delete HKCU\Software\Microsoft\VisualStudio\8.0\ProjectMRUList /va /f
pause
```