@echo off
cls
echo vvvvvvvvvv-BUILD-LOG-vvvvvvvvvv
g++ main.cpp -o main.exe -std=c++11
echo ^^^^^^^^^^^^^^^^^^^^-BUILD-LOG-^^^^^^^^^^^^^^^^^^^^
echo COMPILATION ERRORLEVEL: %errorlevel%
if %errorlevel% EQU 0 (
echo COMPILATION COMPLETE: RUNNING main.exe
start main.exe
)
if %errorlevel% NEQ 0 (
echo ERROR DURING COMPILATION
)