SET SCRIPT_DIR=%~dp0

cd %SCRIPT_DIR%
CALL setenv.cmd
call install_pip.cmd

SET PY_CMD=python.exe
SET PIP_CMD=scripts\pip3 
SET PACKAGE_CMD=setup.py bdist_wheel
SET PACKAGE_SRC_CMD=setup.py sdist

rm -fr dist build


ECHO 64 Bit builds
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

cd %SCRIPT_DIR%

"%PY_64BIT%\%PY_CMD%" %PACKAGE_CMD%
if %ERRORLEVEL% NEQ 0 ( exit /b %errorlevel% )

"%PY_64BIT%\%PY_CMD%" %PACKAGE_SRC_CMD%
if %ERRORLEVEL% NEQ 0 ( exit /b %errorlevel% )

dir dist
echo twine upload dist/*
