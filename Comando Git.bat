
@ECHO OFF

PowerShell.exe -Command "CD D:\PROYECTO JABONES"

git add .
git commit -m "Comando %RANDOM%"
 
git remote add origin https://github.com/hjestrada/Proyecto_Jabones.git
 
git push -u origin master
pauser