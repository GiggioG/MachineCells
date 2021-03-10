@echo off
if exist "main.exe" (
    start cmd /k "main.exe"
    goto end
)
echo NOT COMPILED YET, COMPILE WITH "build"
:end