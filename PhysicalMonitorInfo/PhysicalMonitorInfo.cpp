// PhysicalMonitorInfo.cpp : Defines the entry point for the console application.
//

#include "CStringHelpers.h"
#include "DimensionFetcher.h"

#include "targetver.h"
#include <tchar.h>

#include <iostream>

int _tmain(int argc, _TCHAR* argv[]) {
	short WidthMm, HeightMm;
	int monitorNum = 1;
	if (GetSizeForMonitorNumber(monitorNum, WidthMm, HeightMm)) {
		std::cout << WidthMm << std::endl;
		std::cout << HeightMm << std::endl;
		return 0;
	} else {
		std::cerr << "Failed to get size of monitor number " << monitorNum << std::endl;
		return 1;
	}
}
