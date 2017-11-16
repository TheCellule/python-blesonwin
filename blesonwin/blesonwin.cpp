// blesonwin - Native Python Module for accessing the Windows 10+ BLuetooth LE Functions
// wayne@thebubbleworks.com

// EXPERIMENTAL

// This was startred life as the MS C++ Extentions for Python Example, then C++/WinRT calls were added.
// See: https://docs.microsoft.com/en-gb/visualstudio/python/cpp-and-python
// See: https://github.com/Microsoft/cppwinrt

// From standard example perform these VS2017 project changes changes for C++/WinRT:

// On the C++ project item in Solution Explorer:
// add Properties -> C/C++ -> Additional Include paths: $(ProjectDir)/winrt
// add Properties -> C/C++ -> Commandline -> Extra Flags : /await /std:c++latest 
// add Properties -> C/C++, General tab,  Consume Windows Runtime Extension to Yes (/ZW). 
// add Properties -> C/C++, Code Generation,  Disable Minmal Rebuild 
// add Properties -> C/C++, General tab, Additional Using paths: 
//							C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcpackages
//							C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\lib\store\references
//							C:\Program Files (x86)\Windows Kits\10\UnionMetadata
// TODO: use macros, e.g. $(VCInstallDir), $(WindowsSDKDir)

// Not currently needed on Using path:
// C:\Program Files (x86)\Windows Kits\10\References\Windows.Networking.Connectivity.WwanContract\1.0.0.0
// C:\Program Files (x86)\Windows Kits\10\References\Windows.Foundation.UniversalApiContract\1.0.0.0
// C:\Program Files (x86)\Windows Kits\10\References\Windows.Foundation.FoundationContract\1.0.0.0
// see: https://social.msdn.microsoft.com/Forums/sqlserver/en-US/a6769b36-0390-44c7-93d9-428298a5234d/setting-path-for-windowswinmd-in-visual-studio-2015-when-using-zw-option-to-write-ccx-code-to?forum=visualstudiogeneral


//#pragma comment(lib, "windowsapp")

#include <Windows.h>
#include <iostream>

#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Devices.Bluetooth.h"
#include "winrt/Windows.Devices.Bluetooth.Advertisement.h"

#include <Python.h>

using namespace std;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Devices;
using namespace winrt::Windows::Devices::Bluetooth;


void call_on_advertisement_callback(const Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs advertisement);


Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher bleAdvertisementWatcher = nullptr;
Bluetooth::Advertisement::BluetoothLEAdvertisementPublisher bleAdvertisementPublisher = nullptr;


PyObject* initialise_impl() {
	cout << "Initiliasing C++/WinRT" << endl;
	winrt::init_apartment();

	Py_RETURN_NONE;
}

PyObject* start_observer_impl() {
	cout << "Creating AdvertisementWatcher" << endl;
	bleAdvertisementWatcher = Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher();

	bleAdvertisementWatcher.ScanningMode(Bluetooth::Advertisement::BluetoothLEScanningMode::Active);

	bleAdvertisementWatcher.Received([=](const Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher watcher, const Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs advertisement) {
		cout << "Advertisment Report received..."  <<endl;
		call_on_advertisement_callback(advertisement);
	});

	cout << "Starting AdvertisementWatcher" << endl;
	bleAdvertisementWatcher.Start();

	Py_RETURN_NONE;
}

PyObject* stop_observer_impl() {
	cout << "Stopping AdvertisementWatcher" << endl;
	bleAdvertisementWatcher.Stop();

	Py_RETURN_NONE;
}


// https://github.com/Microsoft/Windows-universal-samples/blob/master/Samples/BluetoothAdvertisement/cs/Scenario2_Publisher.xaml.cs

PyObject* start_advertiser_impl() {
	cout << "Starting AdvertisementPublisher" << endl;
	bleAdvertisementPublisher = Bluetooth::Advertisement::BluetoothLEAdvertisementPublisher();

	Advertisement::BluetoothLEManufacturerData manufacturerData = Advertisement::BluetoothLEManufacturerData();

	manufacturerData.CompanyId(0xFFFE);
	//manufacturerData.Data(0x1234);
	//bleAdvertisementPublisher.Advertisement.ManufacturerData.Add(manufacturerData);

	bleAdvertisementPublisher.Start();
	Py_RETURN_NONE;
}

PyObject* stop_advertiser_impl() {
	cout << "Stopping AdvertisementPublisher" << endl;

	Py_RETURN_NONE;
}


static PyObject* on_advertisement_callback = NULL;

static PyObject* on_advertisement_impl(PyObject *dummy, PyObject *args)
{
	PyObject *result = NULL;
	PyObject *temp;

	if (PyArg_ParseTuple(args, "O:on_advertisement", &temp)) {
		if (!PyCallable_Check(temp)) {
			PyErr_SetString(PyExc_TypeError, "parameter must be callable");
			return NULL;
		}
		Py_XINCREF(temp);         /* Add a reference to new callback */
		Py_XDECREF(on_advertisement_callback);  /* Dispose of previous callback */
		on_advertisement_callback = temp;       /* Remember new callback */
								  /* Boilerplate to return "None" */
		Py_INCREF(Py_None);
		result = Py_None;
	}
	return result;
}

void call_on_advertisement_callback(const Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs advertisement) {
	if (!on_advertisement_callback) {
		return;
	}
	PyGILState_STATE gstate;
	gstate = PyGILState_Ensure();

	PyObject *advertisement_dict = PyDict_New();
	//PyDict_SetItem(advertisement_dict, Py_BuildValue("s", "key1"), Py_BuildValue("i", 123));

	PyObject *arglist;
	//arglist = Py_BuildValue("(o)", advertisement_dict);
	arglist = Py_BuildValue("(i)", advertisement.RawSignalStrengthInDBm());

	PyObject *result;
	result = PyObject_CallObject(on_advertisement_callback, arglist);
	Py_DECREF(advertisement_dict);
	Py_DECREF(arglist);

	if (result == NULL)
		return; // NULL; /* Pass error back */
	Py_DECREF(result);


	// Release the thread. No Python API allowed beyond this point.
	PyGILState_Release(gstate);
}


// Python Native Module definition

static PyMethodDef blesonwin_methods[] = {
	{ "initialise", (PyCFunction)initialise_impl, METH_NOARGS, nullptr },
	{ "start_observer", (PyCFunction)start_observer_impl, METH_NOARGS, nullptr },
	{ "stop_observer", (PyCFunction)stop_observer_impl, METH_NOARGS, nullptr },
	{ "start_advertiser", (PyCFunction)start_advertiser_impl, METH_NOARGS, nullptr },
	{ "stop_advertiser", (PyCFunction)stop_advertiser_impl, METH_NOARGS, nullptr },
	{ "on_advertisement", (PyCFunction)on_advertisement_impl, METH_VARARGS, nullptr },
	{ nullptr, nullptr, 0, nullptr }
};

static PyModuleDef blesonwin_module = {
	PyModuleDef_HEAD_INIT,
	"blesonwin",                        // Module name
	"Provides a BluetoothLE Wrapper",	// Module description
	0,
	blesonwin_methods                   // Structure that defines the methods
};


PyMODINIT_FUNC PyInit_blesonwin() {
	if (!PyEval_ThreadsInitialized()) {
		PyEval_InitThreads();
	}
	return PyModule_Create(&blesonwin_module);
}
