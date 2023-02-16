@echo off

::一些没啥用的设置
title fukin push

:backhome

echo =================
echo =  welcome  !!!
echo = press "asd" to upload 
echo = press any keys to cls
echo =================

set /p inp=your choice:
if "%inp%"=="asd"  (
git add .
git commit -m "update"
git push
echo+
echo+
echo ---new round---
echo+
echo+
) else (
cls
)

goto backhome
pause