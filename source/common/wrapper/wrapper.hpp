#ifndef _COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_
#define _COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_

#include <common/except/SystemError.hpp>

#define CHECK_RETURN_ZERO(...) \
	do { \
		if ((__VA_ARGS__) != 0) { \
			RAISE(SystemError, errno); \
		} \
	} while (false)

#define CHECK_NON_NEGATIVE(type, ...) \
	type value = (__VA_ARGS__); \
	if (value < 0) { \
		RAISE(SystemError, errno); \
	} \
	return value

#include <common/wrapper/libc.hpp>
#include <common/wrapper/unix.hpp>

#undef CHECK_RETURN_ZERO
#undef CHECK_NON_NEGATIVE

#endif // _COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_
