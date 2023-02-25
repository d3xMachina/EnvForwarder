@echo off

rem Example : Change files associations for Media Player Classic BE

rem To change the files associations for another software, replace the value REG_KEY_SEARCH with the corresponding one in regedit HKEY_CLASSES_ROOT
rem To revert, change REG_EXE_NAME to your app name and run it again

set "REG_KEY_SEARCH=mpc-be64."
set "REG_EXE_NAME=EnvForwarder.exe"

rem Check for permissions
if "%PROCESSOR_ARCHITECTURE%" EQU "amd64" (
>nul 2>&1 "%SYSTEMROOT%\SysWOW64\cacls.exe" "%SYSTEMROOT%\SysWOW64\config\system"
) else (
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
)
if "%errorlevel%" NEQ "0" (
    echo Requesting administrative privileges...
    goto UACPrompt
) else (
    goto gotAdmin
)

:UACPrompt
echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
set params= %*
echo UAC.ShellExecute "cmd.exe", "/c ""%~f0"" %params:"=""%", "", "runas", 1 >> "%temp%\getadmin.vbs"

"%temp%\getadmin.vbs"
del "%temp%\getadmin.vbs"
exit /B

:gotAdmin
pushd "%CD%"
cd /d "%~dp0"

echo You are about to change the files associations corresponding to %REG_KEY_SEARCH% to use instead %REG_EXE_NAME%
pause

setlocal EnableDelayedExpansion
for /f "skip=1 tokens=*" %%k in ('reg query HKCR /c /k /f "%REG_KEY_SEARCH%"') do (
	set "key=%%k"
	set "prefix=!key:~0,4!"
	if [!prefix!]==[HKEY] (
		echo Setup %REG_EXE_NAME% for !key!...
		reg add "!key!\shell\open\command" /f /ve /t REG_SZ /d "\"%~dp0%REG_EXE_NAME%\" \"%%1\""
	) else (
		rem echo Skip !key!
	)
)
pause