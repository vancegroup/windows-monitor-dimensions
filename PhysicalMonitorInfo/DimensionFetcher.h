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

/** @brief Look up physical dimensions of a monitor, given its device ID string.
	@param[in] TargetDevID the device ID string associated with a monitor.
	@param[out] WidthMm the monitor physical width in millimeters
	@param[out] HeightMm the monitor physical height in millimeters
	@return true if lookup successful
*/
bool GetSizeForDevID(const CString& TargetDevID, short& WidthMm, short& HeightMm);

/** @brief Look up device ID string and physical dimensions of a monitor, given its number.

	Internally enumerates active monitors to count as display properties does, in order
	to lookup the device ID string and call GetSizeForDevID.

	@param[in] id monitor number, as used by Display Properties > Settings
	@param[out] associatedDeviceID the device ID string associated with that monitor.
	@param[out] WidthMm the monitor physical width in millimeters
	@param[out] HeightMm the monitor physical height in millimeters
	@return true if lookup successful
*/
bool GetSizeForMonitorNumber(int id, CString & associatedDeviceID, short& WidthMm, short& HeightMm);

#endif // INCLUDED_DimensionFetcher_h_GUID_FF784CE8_222C_4EA7_77CE_A49EA6E175C8

