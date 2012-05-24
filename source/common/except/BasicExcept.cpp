#include <cstring>
#include <cstdio>
#include <typeinfo>
#include <common/except/BasicExcept.hpp>

using namespace vision;

BasicExcept::BasicExcept(const char* origin) {
	const char* start = origin + std::strlen(origin) - 1;
	while (start > origin && *start != '/') {
		--start;
	}
	++start;
	snprintf(message, sizeof(message), "%s, %s", start, typeid(*this).name());
}

void BasicExcept::buildMessageBuffer(const char* format, va_list ap) {
	int length = std::strlen(message);
	if (format != NULL && length < MAX_MESSAGE_LENGTH - 1) {
		message[length++] = ' ';
		vsnprintf(message + length, sizeof(message) - length, format, ap);
	}
}
