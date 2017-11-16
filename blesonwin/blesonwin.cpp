// This is the MS C++ Extentions for P{ython Example with with added C++/WinRT calls

// FRom standard example perfomr these VS2017 project changes changes:

// add winrt include. $(ProjectDir)/winrt
// add Properties -> C/C++ -> Commandline -> Extra Flags : /await /std:c++latest 
// add Properties -> C/C++, General tab,  Consume Windows Runtime Extension to Yes (/ZW). 
// add Properties -> C/C++, Code GEneration,  Disable Minmal Rebuild 
//						This enables component extensions (C++/CX).
// add Properties -> C/C++, General tab, Addiotnal Using paths: 
// C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcpackages
// C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\store\references
// C:\Program Files (x86)\Windows Kits\10\UnionMetadata

/* see: https://social.msdn.microsoft.com/Forums/sqlserver/en-US/a6769b36-0390-44c7-93d9-428298a5234d/setting-path-for-windowswinmd-in-visual-studio-2015-when-using-zw-option-to-write-ccx-code-to?forum=visualstudiogeneral
C:\Program Files (x86)\Windows Kits\10\UnionMetadata
C:\Program Files (x86)\Windows Kits\10\References\Windows.Networking.Connectivity.WwanContract\1.0.0.0
C:\Program Files (x86)\Windows Kits\10\References\Windows.Foundation.UniversalApiContract\1.0.0.0
C:\Program Files (x86)\Windows Kits\10\References\Windows.Foundation.FoundationContract\1.0.0.0
*/

//#pragma comment(lib, "windowsapp")

#include <Windows.h>
#include <cmath>    
#include <iostream>

#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Devices.Bluetooth.h"
#include "winrt/Windows.Devices.Bluetooth.Advertisement.h"

#include <Python.h>

using namespace std;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Devices;

const double e = 2.7182818284590452353602874713527;

double sinh_impl(double x) {
	return (1 - pow(e, (-2 * x))) / (2 * pow(e, -x));
}

double cosh_impl(double x) {
	return (1 + pow(e, (-2 * x))) / (2 * pow(e, -x));
}


PyObject* tanh_impl(PyObject *, PyObject* o) {
	double x = PyFloat_AsDouble(o);
	double tanh_x = sinh_impl(x) / cosh_impl(x);

	cout << "Calling WinRT" << endl;
	Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher bleAdvertisementWatcher =  Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher();

	return PyFloat_FromDouble(tanh_x);
}

static PyMethodDef blesonwin_methods[] = {
	// The first property is the name exposed to python, the second is the C++ function name        
	{ "fast_tanh", (PyCFunction)tanh_impl, METH_O, nullptr },

	// Terminate the array with an object containing nulls.
	{ nullptr, nullptr, 0, nullptr }
};

static PyModuleDef blesonwin_module = {
	PyModuleDef_HEAD_INIT,
	"blesonwin",                        // Module name
	"Provides some functions, but faster",  // Module description
	0,
	blesonwin_methods                   // Structure that defines the methods
};

PyMODINIT_FUNC PyInit_blesonwin() {
	return PyModule_Create(&blesonwin_module);
}