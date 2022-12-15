@echo off
echo =================
echo =  welcome  =
echo =================
:backhome
set /p inp=input "asd" and I will upload al your files:
if %inp% equ "asd" （
git add .
git commit -m "push now!"
git push
)
else goto backhome
pause