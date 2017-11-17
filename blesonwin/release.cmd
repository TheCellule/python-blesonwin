REM cd C:\Users\Wayne\source\repos\bleson_win_test\blesonwin
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"

rm -fr dist build
"c:\snakes\python35\python.exe" setup.py sdist bdist_wheel
"C:\Program Files (x86)\Python36-32\python.exe" setup.py sdist bdist_wheel

twine upload dist/*
