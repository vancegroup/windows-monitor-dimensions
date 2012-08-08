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
		DWORD dwType, AcutalValueNameLength = NAME_SIZE;
		TCHAR valueName[NAME_SIZE];

		BYTE EDIDdata[1024];
		DWORD edidsize = sizeof(EDIDdata);

		for (LONG i = 0, retValue = ERROR_SUCCESS; retValue != ERROR_NO_MORE_ITEMS; ++i) {
			retValue = RegEnumValue(hDevRegKey, i, &valueName[0],
			                        &AcutalValueNameLength, NULL, &dwType,
			                        EDIDdata, // buffer
			                        &edidsize); // buffer size

			if (retValue != ERROR_SUCCESS || 0 != _tcscmp(valueName, _T("EDID"))) {
				continue;
			}

			WidthMm  = ((EDIDdata[68] & 0xF0) << 4) + EDIDdata[66];
			HeightMm = ((EDIDdata[68] & 0x0F) << 8) + EDIDdata[67];

			return true; // valid EDID found
		}

		return false; // EDID not found
	}
}

bool GetSizeForDevID(const CString& TargetDevID, short& WidthMm, short& HeightMm) {
	HDEVINFO devInfo = SetupDiGetClassDevsEx(
	                       &GUID_CLASS_MONITOR, //class GUID
	                       NULL, //enumerator
	                       NULL, //HWND
	                       DIGCF_PRESENT, // Flags //DIGCF_ALLCLASSES|
	                       NULL, // device info, create a new one.
	                       NULL, // machine name, local machine
	                       NULL);// reserved

	if (NULL == devInfo) {
		return false;
	}

	bool bRes = false;

	for (ULONG i = 0; ERROR_NO_MORE_ITEMS != GetLastError(); ++i) {
		SP_DEVINFO_DATA devInfoData;
		memset(&devInfoData, 0, sizeof(devInfoData));
		devInfoData.cbSize = sizeof(devInfoData);

		if (SetupDiEnumDeviceInfo(devInfo, i, &devInfoData)) {
			HKEY hDevRegKey = SetupDiOpenDevRegKey(devInfo, &devInfoData,
			                                       DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);

			if (!hDevRegKey || (hDevRegKey == INVALID_HANDLE_VALUE)) {
				continue;
			}

			bRes = GetMonitorSizeFromEDID(hDevRegKey, WidthMm, HeightMm);

			RegCloseKey(hDevRegKey);
		}
	}
	SetupDiDestroyDeviceInfoList(devInfo);
	return bRes;
}


bool GetSizeForMonitorNumber(int id, CString & associatedDeviceID, short& WidthMm, short& HeightMm) {
	DISPLAY_DEVICE dd;
	dd.cb = sizeof(dd);
	DWORD dev = 0; // device index

	CString DeviceID;
	bool bFoundDevice = false;
	while (EnumDisplayDevices(0, dev, &dd, 0) && !bFoundDevice) {
		DISPLAY_DEVICE ddMon;
		ZeroMemory(&ddMon, sizeof(ddMon));
		ddMon.cb = sizeof(ddMon);
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

			ZeroMemory(&ddMon, sizeof(ddMon));
			ddMon.cb = sizeof(ddMon);
		}

		ZeroMemory(&dd, sizeof(dd));
		dd.cb = sizeof(dd);
		dev++;
	}
	return bFoundDevice;
}