/*
 * dropbox_ext4.c
 *
 * Compile like this:
 *   gcc -shared -fPIC -ldl -o libdropbox_ext4.so dropbox_ext4.c
 *
 * Run Dropbox like this:
 *   LD_PRELOAD=./libdropbox_ext4.so ~/.dropbox-dist/dropboxd
 */

#define _GNU_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <sys/vfs.h>
#include <linux/magic.h>
#include <dlfcn.h>
#include <fcntl.h>

static
int (*orig_statfs)(const char *path, struct statfs64 *buf) = NULL;

int statfs64(const char *path, struct statfs64 *buf) {
	if (orig_statfs == NULL) {
		orig_statfs = dlsym(RTLD_NEXT, "statfs64");
	}
	int retval = orig_statfs(path, buf);
	if (retval == 0) {
		buf->f_type = EXT4_SUPER_MAGIC;
	}
	return retval;
}

#if 0
static
int (*orig_posix_fallocate)(int fd, off_t offset, off_t len);

int posix_fallocate(int fd, off_t offset, off_t len)
{
	if (!orig_posix_fallocate)
		orig_posix_fallocate = dlsym(RTLD_NEXT, "posix_fallocate");

	int r = orig_posix_fallocate(fd, offset, len);
	if (r < 0) {
		// eh, it's probably fine.
		fprintf(stderr, "fake_ext4: overriding posix_fallocate() failure\n");
		return 0;
	}

	return 0;
}
#endif

#if 0
static
int (*orig_fallocate)(int fd, int mode, off_t offset, off_t len);

// fallocate(81, 0, 0, 1)                  = -1 EOPNOTSUPP (Operation not supported)
// note: a better choice here would be tracking `open` calls and detect those made
int fallocate(int fd, int mode, off_t offset, off_t len)
{
	if (!orig_fallocate)
		orig_fallocate = dlsym(RTLD_NEXT, "fallocate");

	int r = orig_fallocate(fd, mode, offset, len);
	if (r < 0) {
		// claim we succeeded if goal is allocating
		//
		// TODO: consider if we should do some seek/write to actually do this
		// properly.
		if (mode == 0) {
			return 0;
		}
	}

	return r;
}
#endif

#if 0
static
int (*orig_fstatfs)(int fd, struct statfs *buf) = NULL;

int fstatfs(int fd, struct statfs *buf)
{
	if (!orig_fstatfs)
		orig_fstatfs = dlsym(RTLD_NEXT, "fstatfs");

	int r = orig_fstatfs(fd, buf);
	if (r == 0)
		buf->f_type = EXT4_SUPER_MAGIC;

	return r;
}
#endif
