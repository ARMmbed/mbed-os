:: Path to Elf2Bin tool
::set ELF2BIN=C:/Program Files (x86)/GNU Tools ARM Embedded/7 2018-q2-update/bin/arm-none-eabi-objcopy.exe
set ELF2BIN=%USERPROFILE%\ModusToolbox\tools_2.1\gcc-7.2.1\bin\arm-none-eabi-objcopy.exe

:: Check ELF2BIN
@if not exist "%ELF2BIN%" (
  @echo Error: %ELF2BIN% is not found, exiting
  @exit /b 1
)

:: Check python
@python --version
@if ERRORLEVEL 1 (
  @echo Error: python executable is not found, exiting
  @exit /b 1
)

:: Convert ELF to C
@call :elf2c %* || goto :error
@exit /b 0

:error
@echo Error: failed to convert ELF to C
@exit /b 1

:: This function converts ELF executable to C file
:: Additionally, it generates HEX file
:: arg#1: path to ELF file
:: arg#2: path to C file
:: arg#3: C preprocessor guard macro (optional)
:elf2c
@if "%2" == "" (
  @echo Error: expected 2 or 3 arguments: ^<path-to-elf^> ^<path-to-c^> [c-macro]
  exit /b 1
)
@set ELF_PATH=%1
@set HEX_PATH=%~dpn1.hex
@set BIN_PATH=%~dpn1.bin
@set C_PATH=%2
@set C_MACRO=%3
@if not exist "%ELF_PATH%" (
  @echo Error: %ELF_PATH% is not found, exiting
  goto :eof
)
"%ELF2BIN%" -O ihex "%ELF_PATH%" "%HEX_PATH%" || goto :eof
"%ELF2BIN%" -O binary "%ELF_PATH%" "%BIN_PATH%" || goto :eof
python "%~dp0bin2c.py" "%BIN_PATH%" "%C_PATH%" "%C_MACRO%" || goto :eof
@echo Successfully converted %ELF_PATH% to %C_PATH%
@goto :eof
