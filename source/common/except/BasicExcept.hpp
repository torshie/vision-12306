#ifndef _COMMON_EXCEPT_UNKNOWN_EXCEPTION_HPP_INCLUDED_
#define _COMMON_EXCEPT_UNKNOWN_EXCEPTION_HPP_INCLUDED_

#include <cstddef>
#include <exception>
#include <cstdarg>

namespace vision {

class BasicExcept : public std::exception {
public:
	virtual ~BasicExcept() throw() {}

	const char* what() const throw() {
		return message;
	}

protected:
	enum { MAX_MESSAGE_LENGTH = 256 };
	char message[MAX_MESSAGE_LENGTH];

	BasicExcept(const char* origin);

	void buildMessageBuffer(const char* format, va_list ap);
};

#define _TOKEN_TO_STRING__(token) #token
#define _TOKEN_TO_STRING_(token) _TOKEN_TO_STRING__(token)
#define SOURCE_CODE_ORIGIN __FILE__ ":" _TOKEN_TO_STRING_(__LINE__)

#define RAISE(except, ...) throw except(SOURCE_CODE_ORIGIN, ##__VA_ARGS__)

#define BUILD_MESSAGE_BUFFER(last) \
	do { \
		va_list ap; \
		va_start(ap, last); \
		this->buildMessageBuffer(last, ap); \
		va_end(ap); \
	} while(false)

} // namespace vision

#endif // _COMMON_EXCEPT_UNKNOWN_EXCEPTION_HPP_INCLUDED_
