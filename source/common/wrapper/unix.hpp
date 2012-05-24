#ifndef _COMMON_WRAPPER_UNIX_HPP_INCLUDED_
#define _COMMON_WRAPPER_UNIX_HPP_INCLUDED_

#ifndef _COMMON_WRAPPER_WRAPPER_HPP_INCLUDED_
#	error "#include <common/wrapper/wrapper.hpp> instead"
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <dlfcn.h>

namespace vision { namespace wrapper {

inline DIR* opendir_(const char* name) {
	DIR* result = opendir(name);
	if (result == NULL) {
		RAISE(SystemError, errno);
	}
	return result;
}

inline void closedir_(DIR* dir) {
	CHECK_RETURN_ZERO(closedir(dir));
}

inline pid_t wait_(int* status) {
	CHECK_NON_NEGATIVE(pid_t, wait(status));
}

inline int open_(const char* file, int flags) {
	CHECK_NON_NEGATIVE(int, open(file, flags));
}

inline int open_(const char* file, int flags, mode_t mode) {
	CHECK_NON_NEGATIVE(int, open(file, flags, mode));
}

inline void close_(int fd) {
	CHECK_RETURN_ZERO(close(fd));
}

inline void chown_(const char* path, uid_t owner, gid_t group) {
	CHECK_RETURN_ZERO(chown(path, owner, group));
}

inline void chmod_(const char* path, mode_t mode) {
	CHECK_RETURN_ZERO(chmod(path, mode));
}

inline void fstat_(int fd, struct stat* buf) {
	CHECK_RETURN_ZERO(fstat(fd, buf));
}

inline void munmap_(void* addr, size_t length) {
	CHECK_RETURN_ZERO(munmap(addr, length));
}

inline void* mmap_(void *addr, size_t length, int prot, int flags, int fd,
		off_t offset) {
	void* ret = mmap(addr, length, prot, flags, fd, offset);
	if (ret == MAP_FAILED) {
		RAISE(SystemError, errno);
	}
	return ret;
}

inline int socket_(int domain, int type, int protocol) {
	CHECK_NON_NEGATIVE(int, socket(domain, type, protocol));
}

inline void kill_(pid_t pid, int sig) {
	CHECK_RETURN_ZERO(kill(pid, sig));
}

inline void mkfifo_(const char* path, mode_t mode) {
	CHECK_RETURN_ZERO(mkfifo(path, mode));
}

inline void bind_(int sockfd, const void *addr, socklen_t addrlen) {
	CHECK_RETURN_ZERO(bind(sockfd, (const struct sockaddr*) addr, addrlen));
}

inline void listen_(int sockfd, int backlog) {
	CHECK_RETURN_ZERO(listen(sockfd, backlog));
}

inline int accept_(int sock, struct sockaddr* addr, socklen_t* len) {
	CHECK_NON_NEGATIVE(int, accept(sock, addr, len));
}

inline void connect_(int sockfd, const void *addr, socklen_t addrlen) {
	CHECK_RETURN_ZERO(connect(sockfd, (const struct sockaddr*) addr, addrlen));
}

inline pid_t fork_() {
	CHECK_NON_NEGATIVE(pid_t, fork());
}

inline void chdir_(const char* path) {
	CHECK_RETURN_ZERO(chdir(path));
}

inline off64_t lseek64_(int fd, off64_t offset, int whence) {
	CHECK_NON_NEGATIVE(off64_t, lseek64(fd, offset, whence));
}

inline off_t lseek_(int fd, off_t offset, int whence) {
	CHECK_NON_NEGATIVE(off_t, lseek(fd, offset, whence));
}

inline ssize_t read_(int fd, void* buffer, size_t count) {
	CHECK_NON_NEGATIVE(ssize_t, read(fd, buffer, count));
}

inline ssize_t write_(int fd, const void* buffer, size_t count) {
	CHECK_NON_NEGATIVE(ssize_t, write(fd, buffer, count));
}

inline pid_t waitpid_(pid_t pid, int* status, int flags) {
	pid_t ret = waitpid(pid, status, flags);
	if (ret != pid) {
		RAISE(SystemError, errno);
	}
	return ret;
}

inline ssize_t readlink_(const char *path, char *buf, size_t bufsiz) {
	CHECK_NON_NEGATIVE(ssize_t, readlink(path, buf, bufsiz));
}

inline int dup_(int old) {
	CHECK_NON_NEGATIVE(int, dup(old));
}

inline void setsockopt_(int sockfd, int level, int optname, const void* val,
		socklen_t len) {
	CHECK_RETURN_ZERO(setsockopt(sockfd, level, optname, val, len));
}

inline void unlink_(const char* path) {
	CHECK_RETURN_ZERO(unlink(path));
}

}} // namespace vision::wrapper

#endif // _COMMON_WRAPPER_UNIX_HPP_INCLUDED_
