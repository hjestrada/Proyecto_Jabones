
@ECHO OFF

PowerShell.exe -Command "CD D:\PROYECTO JABONES"

git init
 
git add .
 
git commit -m "first commit"
 
git remote add origin https://github.com/hjestrada/Proyecto_Jabones.git
 
git push -u origin master
pause