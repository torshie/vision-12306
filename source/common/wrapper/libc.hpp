#ifndef _COMMON_WRAPPER_LIBC_HPP_INCLUDED_
#define _COMMON_WRAPPER_LIBC_HPP_INCLUDED_

#ifndef _COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_
#	error "#include <common/wrapper/wrapper.hpp> instead"
#endif

#include <cerrno>
#include <cstdio>

namespace vision { namespace wrapper {

inline void fclose_(FILE* file) {
	CHECK_RETURN_ZERO(std::fclose(file));
}

inline FILE* fopen_(const char* name, const char* mode) {
	FILE* file = std::fopen(name, mode);
	if (file == NULL) {
		RAISE(SystemError, errno);
	}
	return file;
}

inline FILE* fdopen_(int fd, const char* mode) {
	FILE* file = fdopen(fd, mode);
	if (file == NULL) {
		RAISE(SystemError, errno);
	}
	return file;
}

inline size_t fwrite_(const void* buffer, size_t size, size_t count,
		FILE* file) {
	size_t ret = std::fwrite(buffer, size, count, file);
	if (ret != count) {
		RAISE(SystemError, errno);
	}
	return ret;
}

inline size_t fread_(void *ptr, size_t size, size_t count, FILE *stream) {
	size_t ret = std::fread(ptr, size, count, stream);
	if (ret < count && std::ferror(stream)) {
		RAISE(SystemError, errno);
	}
	return ret;
}

inline void fflush_(FILE* file) {
	CHECK_RETURN_ZERO(std::fflush(file));
}

inline void fseek_(FILE *stream, long offset, int whence) {
	CHECK_RETURN_ZERO(std::fseek(stream, offset, whence));
}

}} // namespace vision::wrapper

#endif // _COMMON_WRAPPER_LIBC_HPP_INCLUDED_
