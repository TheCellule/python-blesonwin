SET SCRIPT_DIR=%~dp0

cd %SCRIPT_DIR%
CALL setenv.cmd

SET PY_CMD=python.exe
SET PIP_CMD=scripts\pip3 
SET PACKAGE_CMD=setup.py bdist_wheel
SET PACKAGE_SRC_CMD=setup.py sdist

rm -fr dist build

ECHO 32 Bit builds
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"

cd %SCRIPT_DIR%
"%PY35%\%PY_CMD%" %PACKAGE_CMD%
if %ERRORLEVEL% NEQ 0 ( exit /b %errorlevel% )

"%PY36%\%PY_CMD%" %PACKAGE_CMD%
if %ERRORLEVEL% NEQ 0 ( exit /b %errorlevel% )

ECHO 64 Bit builds
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"

cd %SCRIPT_DIR%
"%PY35_AMD%\%PY_CMD%" %PACKAGE_CMD%
if %ERRORLEVEL% NEQ 0 ( exit /b %errorlevel% )
"%PY36_AMD%\%PY_CMD%" %PACKAGE_CMD%
if %ERRORLEVEL% NEQ 0 ( exit /b %errorlevel% )

"%PY36_AMD%\%PY_CMD%" %PACKAGE_SRC_CMD%
if %ERRORLEVEL% NEQ 0 ( exit /b %errorlevel% )

dir dist
echo twine upload dist/*
