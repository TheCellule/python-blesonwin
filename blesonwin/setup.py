from setuptools import setup, Extension

# Update tools: pip install --upgrade pip setuptools wheel twine


# Prior to running 3.5 python run:
# "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"


blesonwin_module = Extension('blesonwin',                              
                             sources = ['blesonwin.cpp'],
                             extra_compile_args = ["/std:c++latest"], 
                             )

setup(name = 'blesonwin', 
    version = '0.0.6',
    description = 'WIndows 10 WinRT Extension for BluetoothLE',
    license='MIT',
    author='TheCellule',
    author_email='thecellule@gmail.com',
    url='https://github.com/TheCellule/blesonwin',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Programming Language :: Python :: 3.6'
    ],
    ext_modules = [blesonwin_module],
)