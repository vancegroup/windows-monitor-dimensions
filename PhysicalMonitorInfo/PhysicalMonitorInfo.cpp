// PhysicalMonitorInfo.cpp : Defines the entry point for the console application.
//

#include "CStringHelpers.h"
#include "DimensionFetcher.h"

#include "targetver.h"
#include <tchar.h>

#include <iostream>

int _tmain(int argc, _TCHAR* argv[]) {
	std::cout << "Monitor names:" << std::endl;
	dumpMonitorNames();
	std::cout << std::endl;

	short WidthMm, HeightMm;
	CString devID;
	int monitorNum = 1;
	if (GetSizeForMonitorNumber(monitorNum, devID, WidthMm, HeightMm)) {
		std::cout << devID << std::endl;
		std::cout << WidthMm << std::endl;
		std::cout << HeightMm << std::endl;
		return 0;
	} else {
		std::cerr << "Failed to get size of monitor number " << monitorNum << std::endl;
		return 1;
	}
}
