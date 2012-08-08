/** @file
	@brief Implementation

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program


	@author http://thetweaker.wordpress.com/2011/11/13/reading-monitor-physical-dimensions-or-getting-the-edid-the-right-way/

*/

//           Copyright Iowa State University 2012.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include "DimensionFetcher.h"
#include "ScopedHandle/ScopedHandle.h"
#include "ScopedHandle/ScopedHandleSetupAPI.h"
#include "WindowsStructTools.h"
#include "CStringHelpers.h"

// Library/third-party includes
#include <atlstr.h>
#include <SetupApi.h>
#pragma comment(lib, "setupapi.lib")

// Standard includes
// - none

#define NAME_SIZE 128

// Anonymous namespace for internal utilities
namespace {
	const GUID GUID_CLASS_MONITOR = {0x4d36e96e, 0xe325, 0x11ce, 0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18};

	// Assumes hDevRegKey is valid
	bool GetMonitorSizeFromEDID(const HKEY hDevRegKey, short& WidthMm, short& HeightMm) {

		BYTE EDIDdata[1024];
		DWORD edidsize = sizeof(EDIDdata);

		LONG retValue = RegQueryValueEx(hDevRegKey, _T("EDID"), NULL, NULL,  EDIDdata, &edidsize);
		if (retValue != ERROR_SUCCESS) {
			return false;
		}

		WidthMm  = ((EDIDdata[68] & 0xF0) << 4) + EDIDdata[66];
		HeightMm = ((EDIDdata[68] & 0x0F) << 8) + EDIDdata[67];

		return true; // valid EDID found
	}
}

bool GetSizeForDevID(const CString& TargetDevID, short& WidthMm, short& HeightMm) {
	DevInfoScopedHandle devInfo(SetupDiGetClassDevsEx(
	                                &GUID_CLASS_MONITOR, //class GUID
	                                NULL, //enumerator
	                                NULL, //HWND
	                                DIGCF_PRESENT, // Flags //DIGCF_ALLCLASSES|
	                                NULL, // device info, create a new one.
	                                NULL, // machine name, local machine
	                                NULL));// reserved

	if (!devInfo) {
		return false;
	}

	bool bRes = false;

	for (ULONG i = 0; ERROR_NO_MORE_ITEMS != GetLastError(); ++i) {
		SP_DEVINFO_DATA devInfoData;
		initWinSizedStruct(devInfoData);

		if (SetupDiEnumDeviceInfo(devInfo.Get(), i, &devInfoData)) {
			RegKeyScopedHandle hDevRegKey(SetupDiOpenDevRegKey(devInfo.Get(), &devInfoData,
			                              DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ));

			if (!hDevRegKey) {
				continue;
			}

			bRes = GetMonitorSizeFromEDID(hDevRegKey.Get(), WidthMm, HeightMm);

		}
	}
	return bRes;
}


template<typename T>
inline void ForEachMonitor(T op) {
	DISPLAY_DEVICE dd;
	initWinSizedStruct(dd);
	DWORD dev = 0; // device index

	CString DeviceID;
	while (EnumDisplayDevices(0, dev, &dd, 0)) {
		DISPLAY_DEVICE ddMon;
		initWinSizedStruct(ddMon);
		DWORD devMon = 0;

		while (EnumDisplayDevices(dd.DeviceName, devMon, &ddMon, 0)) {
			if (ddMon.StateFlags & DISPLAY_DEVICE_ACTIVE &&
			        !(ddMon.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)) {
				DeviceID.Format(L"%s", ddMon.DeviceID);
				DeviceID = DeviceID.Mid(8, DeviceID.Find(L"\\", 9) - 8);
				op(ddMon, DeviceID);
			}
			devMon++;
			initWinSizedStruct(ddMon);
		}

		initWinSizedStruct(dd);
		dev++;
	}

}
namespace {
	void dumpName(DISPLAY_DEVICE &, CString & id) {
		std::cout << id << std::endl;
	}
}
void dumpMonitorNames() {
	ForEachMonitor(&dumpName);
}

bool GetSizeForMonitorNumber(int id, CString & associatedDeviceID, short& WidthMm, short& HeightMm) {
	DISPLAY_DEVICE dd;
	initWinSizedStruct(dd);
	DWORD dev = 0; // device index

	CString DeviceID;
	bool bFoundDevice = false;
	while (EnumDisplayDevices(0, dev, &dd, 0) && !bFoundDevice) {
		DISPLAY_DEVICE ddMon;
		initWinSizedStruct(ddMon);
		DWORD devMon = 0;

		while (EnumDisplayDevices(dd.DeviceName, devMon, &ddMon, 0) && !bFoundDevice) {
			if (ddMon.StateFlags & DISPLAY_DEVICE_ACTIVE &&
			        !(ddMon.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)) {
				DeviceID.Format(L"%s", ddMon.DeviceID);
				DeviceID = DeviceID.Mid(8, DeviceID.Find(L"\\", 9) - 8);
				bFoundDevice = GetSizeForDevID(DeviceID, WidthMm, HeightMm);
				if (bFoundDevice) {
					associatedDeviceID = DeviceID;
				}
			}
			devMon++;

			initWinSizedStruct(ddMon);
		}

		initWinSizedStruct(dd);
		dev++;
	}
	return bFoundDevice;
}