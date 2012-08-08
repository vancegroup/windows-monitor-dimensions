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
#ifndef INCLUDED_WindowsStructTools_h_GUID_8DCEEA55_8175_4D8F_AE1A_B4ABC4225AD3
#define INCLUDED_WindowsStructTools_h_GUID_8DCEEA55_8175_4D8F_AE1A_B4ABC4225AD3


// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
// - none

namespace detail {
	// Based on http://stackoverflow.com/a/1007175
#define CREATE_HAS_MEMBER(MEMNAME, TRAITNAME) \
	template<typename T> struct TRAITNAME { \
		struct Fallback { int MEMNAME; };  \
		struct Derived : T, Fallback { }; \
		template<typename C, C> struct ChT; \
		template<typename C> static char (&f(ChT<int Fallback::*, &C::MEMNAME>*))[1]; \
		template<typename C> static char (&f(...))[2]; \
		static bool const value = sizeof(f<Derived>(0)) == 2; \
	};

	CREATE_HAS_MEMBER(cbSize, has_cbSize)
	CREATE_HAS_MEMBER(cb, has_cb)
#undef CREATE_HAS_MEMBER

	template<bool, typename Result = void>
	struct enable_if_c {};

	template<typename Result>
	struct enable_if_c<true, Result> {
		typedef Result type;
	};

	template<typename T, typename Result = void>
	struct enable_if : enable_if_c<T::value, Result> {};

	/// @name setSizeField variants
	/// @{
	template<typename T>
	static inline typename enable_if<has_cb<T>, void>::type setSizeField(T & v) {
		v.cb = sizeof(v);
	}

	template<typename T>
	static inline typename enable_if<has_cbSize<T>, void>::type setSizeField(T & v) {
		v.cbSize = sizeof(v);
	}

	template<typename T>
	static inline typename enable_if_c < !has_cbSize<T>::value && !has_cb<T>::value, void >::type setSizeField(T &) {
		// no-op
	}
	///@}
} //end of namespace detail

template<typename T>
inline void initWinSizedStruct(T & v) {
	memset(&v, 0, sizeof(v));
	detail::setSizeField(v);
}
#endif // INCLUDED_WindowsStructTools_h_GUID_8DCEEA55_8175_4D8F_AE1A_B4ABC4225AD3

