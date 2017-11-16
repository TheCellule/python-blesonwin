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

Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher bleAdvertisementWatcher = nullptr;
Bluetooth::Advertisement::BluetoothLEAdvertisementPublisher bleAdvertisementPublisher = nullptr;


// C# Sample ref: https://github.com/Microsoft/Windows-universal-samples/blob/master/Samples/BluetoothAdvertisement/cs/Scenario2_Publisher.xaml.cs
void test_advertiser() {
	cout << "Creating AdvertisementPublisher" << endl;
	bleAdvertisementPublisher = Bluetooth::Advertisement::BluetoothLEAdvertisementPublisher();

	Advertisement::BluetoothLEManufacturerData manufacturerData = Advertisement::BluetoothLEManufacturerData();
	manufacturerData.CompanyId(0xFFFE);
	//manufacturerData.Data(0x1234);
	//bleAdvertisementPublisher.Advertisement.ManufacturerData.Add(manufacturerData);
	
	cout << "Starting Advertiser" << endl;
	bleAdvertisementPublisher.Start();

	cout << "Hit enter to stop advertising." << endl;
	cin.ignore();

	cout << "Stopping Advertiser" << endl;
	bleAdvertisementPublisher.Stop();
}


// C# Sample ref: https://github.com/Microsoft/Windows-universal-samples/blob/master/Samples/BluetoothAdvertisement/cs/Scenario1_Watcher.xaml.cs
void test_watcher() {
	cout << "Creating Advertisement Watcher" << endl;
	bleAdvertisementWatcher = Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher();

	cout << "Starting Watcher" << endl;
	bleAdvertisementWatcher.Start();

	cout << "Hit enter to stop watching." << endl;
	cin.ignore();

	cout << "Stopping Watcher" << endl;
	bleAdvertisementWatcher.Stop();
}


int main(int argc, char *argv[], char *envp[]) {
	winrt::init_apartment();
	
	//test_advertiser();
	test_watcher();
}
