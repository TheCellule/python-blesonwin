from distutils.core import setup, Extension, DEBUG

blesonwin_module = Extension('blesonwin',                              
                             sources = ['blesonwin.cpp'],
                             extra_compile_args = ["/std:c++latest"], 
                             )

setup(name = 'blesonwin', version = '0.1',
    description = 'Python Package with WinRT C++ Extension for BluetoothLE',
    ext_modules = [blesonwin_module],
    )