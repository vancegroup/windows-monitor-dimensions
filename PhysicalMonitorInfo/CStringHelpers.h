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
#ifndef INCLUDED_CStringHelpers_h_GUID_7DB5BF31_5D98_4D9C_B9DE_D6AC444C9F81
#define INCLUDED_CStringHelpers_h_GUID_7DB5BF31_5D98_4D9C_B9DE_D6AC444C9F81


// Internal Includes
// - none

// Library/third-party includes
#include <atlstr.h>

// Standard includes
#include <iostream>
#include <stdexcept>

namespace {
	template<class StreamType, class StringType>
	inline StreamType & output(StreamType & os, StringType & str) {
		os << str.GetBuffer(str.GetLength());
		str.ReleaseBuffer();
		return os;
	}
}

template<class StringTraits>
inline std::ostream & operator<<(std::ostream & os, CStringT<char, StringTraits> & str) {
	return output<std::ostream>(os, str);
}

template<class StringTraits>
inline std::wostream & operator<<(std::wostream & os, CStringT<wchar_t, StringTraits> & str) {
	return output<std::wostream>(os, str);
}

template<class StringTraits>
inline std::wostream & operator<<(std::ostream & os, CStringT<wchar_t, StringTraits> & str) {
	std::wostream * widestream = NULL;
	if (&os == &static_cast<std::ostream&>(std::cout)) {
		widestream = &std::wcout;
	} else if (&os == &static_cast<std::ostream&>(std::cerr)) {
		widestream = &std::wcerr;
	} else if (&os == &static_cast<std::ostream&>(std::clog)) {
		widestream = &std::wclog;
	} else {
		throw std::logic_error("Trying to stream a wide CString to a narrow stream, and it's not a built-in one that we could find the equivalent of!");
	}
	return output<std::wostream>(*widestream, str);
}

#endif // INCLUDED_CStringHelpers_h_GUID_7DB5BF31_5D98_4D9C_B9DE_D6AC444C9F81

