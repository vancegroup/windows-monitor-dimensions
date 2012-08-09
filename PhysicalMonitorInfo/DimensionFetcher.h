/** @file
	@brief Header

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

#pragma once
#ifndef INCLUDED_DimensionFetcher_h_GUID_FF784CE8_222C_4EA7_77CE_A49EA6E175C8
#define INCLUDED_DimensionFetcher_h_GUID_FF784CE8_222C_4EA7_77CE_A49EA6E175C8


// Internal Includes
#include "targetver.h"

// Library/third-party includes
#include <atlstr.h>

// Standard includes
// - none

void dumpMonitorNames();

/** @brief Look up physical dimensions of a monitor, given its hardware ID string.
	@param[in] TargetHardwareID the hardware ID string associated with a monitor.
	@param[out] WidthMm the monitor physical width in millimeters
	@param[out] HeightMm the monitor physical height in millimeters
	@return true if lookup successful
*/
bool GetSizeForHardwareID(const CString& TargetHardwareID, short& WidthMm, short& HeightMm);

CString findHardwareIDForMonitorNumber(int id);

/** @brief Look up device ID string and physical dimensions of a monitor, given its number.

	Internally enumerates active monitors to count as display properties does, in order
	to lookup the hardware ID string and call GetSizeForHardwareID.

	@param[in] id monitor number, as used by Display Properties > Settings
	@param[out] WidthMm the monitor physical width in millimeters
	@param[out] HeightMm the monitor physical height in millimeters
	@return true if lookup successful
*/
bool GetSizeForMonitorNumber(int id, short& WidthMm, short& HeightMm);

#endif // INCLUDED_DimensionFetcher_h_GUID_FF784CE8_222C_4EA7_77CE_A49EA6E175C8

