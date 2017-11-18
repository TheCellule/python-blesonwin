// blesonwin - Native Python Module for accessing the Windows 10+ BLuetooth LE Functions

// wayne@thebubbleworks.com

// EXPERIMENTAL

// This startred life as the MS C++ Extentions for Python Example, then C++/WinRT library and CPython calls were added.
// See: https://docs.microsoft.com/en-gb/visualstudio/python/cpp-and-python
// See: https://github.com/Microsoft/cppwinrt

// From the standard example perform these VS2017 project changes changes for C++/WinRT:

// On the C++ 'blesonwin' project item in Solution Explorer:
// add Properties -> C/C++ -> Additional Include paths: $(ProjectDir)/winrt
// add Properties -> C/C++, General tab,  Consume Windows Runtime Extension to Yes (/ZW). 
// Project Properties -> C/C++ -> CommandLine -> Extra Flags : /await /std:c++latest 
// Project Properties -> C/C++ -> General -> Additional Include paths: $(ProjectDir)/winrt
// Project Properties -> C/C++ -> General -> Additional Using paths: $(VCInstallDir)vcpackages;$(VCInstallDir)lib\store\references;$(WindowsSDKDir)UnionMetadata
// Copy winrt folder to project root as '$(ProjectDir)/winrt' from https://github.com/Microsoft/cppwinrt/tree/master/10.0.16299.0/winrt



// Not currently needed on the 'Additional Using' path:
// C:\Program Files (x86)\Windows Kits\10\References\Windows.Networking.Connectivity.WwanContract\1.0.0.0
// C:\Program Files (x86)\Windows Kits\10\References\Windows.Foundation.UniversalApiContract\1.0.0.0
// C:\Program Files (x86)\Windows Kits\10\References\Windows.Foundation.FoundationContract\1.0.0.0
// see: https://social.msdn.microsoft.com/Forums/sqlserver/en-US/a6769b36-0390-44c7-93d9-428298a5234d/setting-path-for-windowswinmd-in-visual-studio-2015-when-using-zw-option-to-write-ccx-code-to?forum=visualstudiogeneral

// Also taken care of by the /ZW flag set above, so helpful to keep for the setup.py
#pragma comment(lib, "windowsapp")

#include <Windows.h>
#include <string>
#include <iostream>
#include <sstream> 
#include <iomanip>

// for wcout
#include <io.h> 
#include <fcntl.h>

#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Devices.Bluetooth.h"
#include "winrt/Windows.Devices.Bluetooth.Advertisement.h"

#include <Python.h>

using namespace std;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Devices;
using namespace winrt::Windows::Devices::Bluetooth;


void call_on_advertisement_callback(const Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs advertisementRecvEvent);
std::string formatBluetoothAddress(unsigned long long BluetoothAddress);

Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher bleAdvertisementWatcher = nullptr;
Bluetooth::Advertisement::BluetoothLEAdvertisementPublisher bleAdvertisementPublisher = nullptr;


PyObject* initialise_impl() {
	//cout << "Initiliasing C++/WinRT" << endl;
	winrt::init_apartment();

	// for wcout
	//_setmode(_fileno(stdout), _O_U16TEXT);


	Py_RETURN_NONE;
}

PyObject* start_observer_impl() {
	//cout << "Creating AdvertisementWatcher" << endl;
	bleAdvertisementWatcher = Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher();

	bleAdvertisementWatcher.ScanningMode(Bluetooth::Advertisement::BluetoothLEScanningMode::Active);

	bleAdvertisementWatcher.Received([=](const Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher watcher, const Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs advertisement) {
		//cout << "Advertisment Report received..."  <<endl;
		call_on_advertisement_callback(advertisement);
	});

	//cout << "Starting AdvertisementWatcher" << endl;
	bleAdvertisementWatcher.Start();

	Py_RETURN_NONE;
}

PyObject* stop_observer_impl() {
	//cout << "Stopping AdvertisementWatcher" << endl;
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

// Passed : https://docs.microsoft.com/en-us/uwp/api/windows.devices.bluetooth.advertisement.bluetoothleadvertisementreceivedeventargs
// (which is NOT an Advertising Report, the ADvReport is a member of the Event, and is documented here:
// https://docs.microsoft.com/en-us/uwp/api/windows.devices.bluetooth.advertisement.bluetoothleadvertisement

void call_on_advertisement_callback(const Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs advertisementRecvEvent) {
	if (!on_advertisement_callback) {
		return;
	}
	int rssi = advertisementRecvEvent.RawSignalStrengthInDBm();
	auto advType = advertisementRecvEvent.AdvertisementType();
	auto advertisement = advertisementRecvEvent.Advertisement();
	auto serviceUUIDs = advertisement.ServiceUuids();				// aggregates all 16, 32 & 128 bit UUID's into this list!

	auto localName = advertisement.LocalName().data();
	
	auto bdaddr              = advertisementRecvEvent.BluetoothAddress();
	const char* bdaddr_c	 = formatBluetoothAddress(bdaddr).c_str();
	
	//PyObject* bdaddr_list_obj= PyList_New(6);
	//for (size_t i = 0; i < 6; ++i) {
	//	PyList_SetItem(bdaddr_list_obj, i, Py_BuildValue("i", ((bdaddr >> (i * 8)) & 0xff)));
	//}

	PyGILState_STATE gstate;
	gstate = PyGILState_Ensure();

	PyObject* bdaddr_obj = Py_BuildValue("s", bdaddr_c);																	
	PyObject* localname_obj = PyUnicode_FromWideChar(localName, wcslen(localName));			// https://docs.python.org/3/c-api/unicode.html


	PyObject* advertisement_dict = PyDict_New();
	PyDict_SetItem(advertisement_dict, Py_BuildValue("s", "RSSI"), Py_BuildValue("i", rssi));
	PyDict_SetItem(advertisement_dict, Py_BuildValue("s", "ADDRESS"), Py_BuildValue("O", bdaddr_obj));
	//PyDict_SetItem(advertisement_dict, Py_BuildValue("s", "ADDRESS"), Py_BuildValue("O", bdaddr_list_obj));
	PyDict_SetItem(advertisement_dict, Py_BuildValue("s", "LOCALNAME"), Py_BuildValue("O", localname_obj));
	
	// Make the call!
	PyObject* arglist = Py_BuildValue("(O)", advertisement_dict);
	PyObject *result = PyObject_CallObject(on_advertisement_callback, arglist);

	if (advertisement_dict)
		Py_DECREF(advertisement_dict);

	if (arglist)
		Py_DECREF(arglist);

	if (result)
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


// Utils

std::string formatBluetoothAddress(unsigned long long bdaddr) {
	std::stringstream ss;
	ss << std::setw(2) << std::hex << setfill('0') << static_cast<unsigned int>((bdaddr >> (5 * 8)) & 0xff);
	for (size_t i = 1; i < 6; ++i) {
		ss << ':' << std::setw(2) << std::hex << static_cast<unsigned int>((bdaddr >> ((5-i) * 8)) & 0xff);
	}
	return ss.str();
}
