#ifndef _COMMON_EXCEPT_BUFFER_OVERFLOW_HPP_INCLUDED_
#define _COMMON_EXCEPT_BUFFER_OVERFLOW_HPP_INCLUDED_

#include <cstring>
#include <common/except/BasicExcept.hpp>

namespace vision {

class BufferOverflow : public BasicExcept {
public:
	BufferOverflow(const char* origin, const char* format = NULL, ...)
			: BasicExcept(origin) {
		BUILD_MESSAGE_BUFFER(format);
	}
};

} // namespace vision

#endif // _COMMON_EXCEPT_BUFFER_OVERFLOW_HPP_INCLUDED_
