#ifndef _COMMON_EXCEPT_SYSTEM_ERROR_HPP_INCLUDED_
#define _COMMON_EXCEPT_SYSTEM_ERROR_HPP_INCLUDED_

#include <cstdio>
#include <cstring>
#include <common/except/BasicExcept.hpp>

namespace vision {

class SystemError : public BasicExcept {
public:
	SystemError(const char* origin, int errorCode,
			const char* format = NULL, ...)
			: BasicExcept(origin) {
		BUILD_MESSAGE_BUFFER(format);
		int length = std::strlen(message);
		snprintf(message + length, sizeof(message) - length, ", %s",
				std::strerror(errorCode));
	}
};

} // namespace vision

#endif // _COMMON_EXCEPT_SYSTEM_ERROR_HPP_INCLUDED_
