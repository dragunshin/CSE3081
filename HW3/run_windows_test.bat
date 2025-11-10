@echo off
chcp 65001 >nul

echo ========================================================
echo HW3 Quick Sort Benchmark - Windows
echo Testing 5 functions x 6 datasets
echo ========================================================
echo.

REM 컴파일
echo [Compiling...]
g++ -O2 -std=c++11 -o HW3_test.exe HW3_S20211052.cpp my_quick_sorts.cpp
if errorlevel 1 (
    echo Compilation failed!
    pause
    exit /b 1
)
echo Compilation successful!
echo.

REM 테스트할 데이터셋
echo ========================================
echo Testing 1K (1,024 records)
echo ========================================
for %%f in (1 21 22 23 24) do (
    echo Function %%f...
    (
        echo %%f
        echo 1024
        echo input_1024.bin
        echo output_test.bin
    ) > HW3_commands.txt
    HW3_test.exe < HW3_commands.txt | findstr "Time"
)
echo.

echo ========================================
echo Testing 10K (10,240 records)
echo ========================================
for %%f in (1 21 22 23 24) do (
    echo Function %%f...
    (
        echo %%f
        echo 10240
        echo input_10240.bin
        echo output_test.bin
    ) > HW3_commands.txt
    HW3_test.exe < HW3_commands.txt | findstr "Time"
)
echo.

echo ========================================
echo Testing 100K (102,400 records)
echo ========================================
for %%f in (1 21 22 23 24) do (
    echo Function %%f...
    (
        echo %%f
        echo 102400
        echo input_102400.bin
        echo output_test.bin
    ) > HW3_commands.txt
    HW3_test.exe < HW3_commands.txt | findstr "Time"
)
echo.

echo ========================================
echo Testing 500K (524,288 records)
echo ========================================
for %%f in (1 21 22 23 24) do (
    echo Function %%f...
    (
        echo %%f
        echo 524288
        echo input_524288.bin
        echo output_test.bin
    ) > HW3_commands.txt
    HW3_test.exe < HW3_commands.txt | findstr "Time"
)
echo.

echo ========================================
echo Testing 8M (8,388,608 records)
echo ========================================
for %%f in (1 21 22 23 24) do (
    echo Function %%f...
    (
        echo %%f
        echo 8388608
        echo input_8388608.bin
        echo output_test.bin
    ) > HW3_commands.txt
    HW3_test.exe < HW3_commands.txt | findstr "Time"
)
echo.

echo ========================================
echo Testing data.bin (7,777,777 records)
echo ========================================
for %%f in (1 21 22 23 24) do (
    echo Function %%f...
    (
        echo %%f
        echo 7777777
        echo data.bin
        echo output_test.bin
    ) > HW3_commands.txt
    HW3_test.exe < HW3_commands.txt | findstr "Time"
)
echo.

echo ========================================
echo All tests completed!
echo ========================================
del HW3_commands.txt
del output_test.bin
pause
