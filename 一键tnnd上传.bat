@echo off
echo =================
echo =  welcome  =
echo 
echo =================
:backhome
set /p inp=input "asd" and I will upload al your files:
if %inp% equ "asd" （
git add .
git commit -m "push now!"
git push
)
goto backhome
pause