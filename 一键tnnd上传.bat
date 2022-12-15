@echo off

::一些没啥用的设置
title fukin push

:backhome

echo =================
echo =  welcome  !!!
echo = press "enter" to upload 
echo =================

set /p inp=ok,just press enter here!!!
if "%inp%"=="asd" （
git add .
git commit -m "push now!"
git push
echo+
echo+
echo ---new round---
echo+
echo+
)

goto backhome
pause