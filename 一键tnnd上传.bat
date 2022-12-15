@echo off

::一些没啥用的设置
title fukin push

:backhome

echo =================
echo =  welcome  =
echo = 1.asd (upload your files immediately)
echo = 2.qwe (show the details of this program)
echo =================

set /p inp=input your choice:
if %inp%==asd （
	git add .
	git commit -m "push now!"
	git push
	cls
)

goto backhome
pause