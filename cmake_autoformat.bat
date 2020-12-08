@echo off
echo ---------------------------------------------------------
echo Autoformatting all CMake files
echo ---------------------------------------------------------

for /f "tokens=*" %%g in ('dir /s /b *.cmake') do (
    cmake-format --check %%g 2>nul
    if ERRORLEVEL 1 (
        echo Reformatting %%g
        cmake-format -i %%g
    )
)

for /f "tokens=*" %%g in ('dir /s /b CMakeLists.txt') do (
    cmake-format --check %%g 2>nul
    if ERRORLEVEL 1 (
        echo Reformatting %%g
        cmake-format -i %%g
    )
)

echo Auto-format complete.