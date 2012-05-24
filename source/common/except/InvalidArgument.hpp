#ifndef _COMMON_EXCEPT_INVALID_ARGUMENT_HPP_INCLUDED_
#define _COMMON_EXCEPT_INVALID_ARGUMENT_HPP_INCLUDED_

#include <common/except/BasicExcept.hpp>

namespace vision {

class InvalidArgument : public BasicExcept {
public:
	InvalidArgument(const char* origin, const char* format = NULL, ...)
			: BasicExcept(origin) {
		BUILD_MESSAGE_BUFFER(format);
	}
};

} // namespace vision

#endif // _COMMON_EXCEPT_INVALID_ARGUMENT_HPP_INCLUDED_
