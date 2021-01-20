@echo off
echo VVVVVVVVVV-BUILD-LOG-VVVVVVVVVV
g++ main.cpp -o main.exe -std=c++11
echo AAAAAAAAAA-BUILD-LOG-AAAAAAAAAA
echo COMPILATION ERRORLEVEL: %errorlevel%
if %errorlevel% EQU 0 (
echo COMPILATION COMPLETE: RUNNING main.exe
start main.exe
)
if %errorlevel% NEQ 0 (
echo ERROR DURING COMPILATION
)