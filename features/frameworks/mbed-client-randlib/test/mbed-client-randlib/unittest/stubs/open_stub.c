#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>

bool allow_open = true;
int __real_open(const char *path, int flags, ...);
int __wrap_open(const char *path, int flags, ...)
{
	if (allow_open) {
		if (flags & O_CREAT) {
			va_list vl;
			va_start(vl,flags);
			mode_t mode = va_arg(vl, mode_t);
			va_end(vl);
			return __real_open(path, flags, mode);
		} else {
			return __real_open(path, flags);
		}
	} else {
		return -1;
	}
}
