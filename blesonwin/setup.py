from setuptools import setup, Extension

# Author: wayne@thebubbleworks.com


blesonwin_module = Extension('blesonwin',                              
                             sources = ['blesonwin.cpp'],
                             extra_compile_args = ["/std:c++latest"], 
                             )

setup(name = 'blesonwin', 
    version = '0.0.9',
    description = 'Windows 10 WinRT Extension for BluetoothLE',
    license='MIT',
    author='TheCellule',
    author_email='thecellule@gmail.com',
    url='https://github.com/TheCellule/blesonwin',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.5'
    ],
    ext_modules = [blesonwin_module],
)