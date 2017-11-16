// Standalone Windows Console App to call WinRT Bluetooth Advertiser and Scanner API's using the 'cppwinrt' library
//
// Author: wayne@thebubbleworks.com
//
// Project creation Receipe:
//
// Create Empty C++ Project
// Create this file (main.cpp)
// Copy winrt folder to project root as '$(ProjectDir)/winrt' from https://github.com/Microsoft/cppwinrt/tree/master/10.0.16299.0/winrt
// Project Properties -> C/C++ -> CommandLine -> Extra Flags : /await /std:c++latest 
// Project Properties -> C/C++ -> General -> Additional Include paths: $(ProjectDir)/winrt
// Project Properties -> C/C++ -> General -> Additional Using paths: $(VCInstallDir)vcpackages;$(VCInstallDir)lib\store\references;$(WindowsSDKDir)UnionMetadata

// cppwinrt usage ref:
// https://github.com/Microsoft/cppwinrt/blob/master/Docs/Migrating%20C%2B%2B%20CX%20source%20code%20to%20C%2B%2B%20WinRT.md

#pragma comment(lib, "windowsapp")

#include <iostream>

#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Devices.Bluetooth.h"
#include "winrt/Windows.Devices.Bluetooth.Advertisement.h"


using namespace std;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Devices;
using namespace winrt::Windows::Devices::Bluetooth;

// "you never use 'ref new' (or 'new') to allocate a C++/WinRT type. Instead you allocate the value on the stack or as a field of an object."
// see cppwinrt usage ref link above.
Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher bleAdvertisementWatcher = nullptr;
Bluetooth::Advertisement::BluetoothLEAdvertisementPublisher bleAdvertisementPublisher = nullptr;


// API ref: https://docs.microsoft.com/en-us/uwp/api/windows.devices.bluetooth.advertisement.bluetoothleadvertisementpublisher

// C# Sample ref: https://github.com/Microsoft/Windows-universal-samples/blob/master/Samples/BluetoothAdvertisement/cs/Scenario2_Publisher.xaml.cs

// Issue: https://social.msdn.microsoft.com/Forums/sqlserver/en-US/5351a1f0-92f3-498b-a0c1-805d568cb55c/when-uwp-ble-advertising-watcher-is-started-its-status-is-aborted?forum=wdk

void test_advertiser() {
	cout << "Creating AdvertisementPublisher" << endl;

	Bluetooth::Advertisement::BluetoothLEAdvertisement advertisement = Bluetooth::Advertisement::BluetoothLEAdvertisement();
	
	wstring name = L"bleson";
	advertisement.LocalName(name);
		
	bleAdvertisementPublisher = Bluetooth::Advertisement::BluetoothLEAdvertisementPublisher(advertisement);

	//Advertisement::BluetoothLEManufacturerData manufacturerData = Advertisement::BluetoothLEManufacturerData();
	//manufacturerData.CompanyId(0xFFFE);
	//manufacturerData.Data(0x1234);
	//bleAdvertisementPublisher.Advertisement.ManufacturerData.Add(manufacturerData);

	cout << "Starting Advertiser" << endl;
	bleAdvertisementPublisher.Start();

	cout << "Hit enter to stop advertising." << endl;
	cin.ignore();

	cout << "Stopping Advertiser" << endl;
	bleAdvertisementPublisher.Stop();
}


// C# Sample refs: 

// Foreground: https://github.com/Microsoft/Windows-universal-samples/blob/master/Samples/BluetoothAdvertisement/cs/Scenario1_Watcher.xaml.cs
// Background: https://github.com/Microsoft/Windows-universal-samples/blob/master/Samples/BluetoothAdvertisement/cs/Scenario3_BackgroundWatcher.xaml.cs

// Background needs Application permission, so unlikely to work for Python.

// C++ ref: https://github.com/urish/win-ble-cpp/blob/master/BLEScanner/BLEScanner.cpp#L84

void test_watcher() {
	cout << "Creating Advertisement Watcher, a dot will appear for every Advertisemnt report received." << endl;
	bleAdvertisementWatcher = Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher();

	bleAdvertisementWatcher.ScanningMode(Bluetooth::Advertisement::BluetoothLEScanningMode::Active);

	bleAdvertisementWatcher.Received([=](auto &&, auto &&) {
		cout << ".";
	});


	cout << "Starting Watcher" << endl;
	bleAdvertisementWatcher.Start();

	cout << "Hit enter to stop watching." << endl;
	cin.ignore();

	cout << "Stopping Watcher" << endl;
	bleAdvertisementWatcher.Stop();
}


int main(int argc, char *argv[], char *envp[]) {
	winrt::init_apartment();
	
	test_watcher();
	test_advertiser();
}
