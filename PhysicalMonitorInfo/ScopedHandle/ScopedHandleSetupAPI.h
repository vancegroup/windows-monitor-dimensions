/** @file
	@brief Header

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program

*/

//           Copyright Iowa State University 2012.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_ScopedHandleSetupAPI_h_GUID_A36FECCE_89C5_448F_322E_4EE2E34915E5
#define INCLUDED_ScopedHandleSetupAPI_h_GUID_A36FECCE_89C5_448F_322E_4EE2E34915E5


// Internal Includes
#include "ScopedHandle.h"

// Library/third-party includes
#include <SetupApi.h>

// Standard includes
// - none

namespace ScopedHandlePolicies {

	class DestroyDeviceInfoList {
		public:
			static inline void Close(HDEVINFO Handle) {
				SetupDiDestroyDeviceInfoList(Handle);
			}
	};
} // end of namespace ScopedHandlePolicies

/// The wrapper for the device info handle returned from SetupDiGetClassDevsEx call
typedef ScopedHandle<HDEVINFO, ScopedHandlePolicies::InvalidHandleValue, ScopedHandlePolicies::DestroyDeviceInfoList> DevInfoScopedHandle;


#endif // INCLUDED_ScopedHandleSetupAPI_h_GUID_A36FECCE_89C5_448F_322E_4EE2E34915E5

