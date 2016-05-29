/**
 * @file fuse/fuse.h
 * WinFsp FUSE compatible API.
 *
 * This file is derived from libfuse/include/fuse.h:
 *     FUSE: Filesystem in Userspace
 *     Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>
 *
 * @copyright 2015-2016 Bill Zissimopoulos
 */
/*
 * This file is part of WinFsp.
 *
 * You can redistribute it and/or modify it under the terms of the
 * GNU Affero General Public License version 3 as published by the
 * Free Software Foundation.
 *
 * Licensees holding a valid commercial license may use this file in
 * accordance with the commercial license agreement provided with the
 * software.
 */

#ifndef FUSE_H_
#define FUSE_H_

#include "fuse_common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct fuse;

enum fuse_readdir_flags
{
    FUSE_READDIR_PLUS = (1 << 0),
};

enum fuse_fill_dir_flags
{
    FUSE_FILL_DIR_PLUS = (1 << 1),
};

typedef int (*fuse_fill_dir_t)(void *buf, const char *name,
    const struct fuse_stat *stbuf, fuse_off_t off, enum fuse_fill_dir_flags flags);

struct fuse_operations
{
    unsigned int flag_nopath:1;
    unsigned int flag_reserved:31;
    int (*getattr)(const char *path, struct fuse_stat *stbuf);
    int (*readlink)(const char *path, char *buf, size_t size);
    int (*mknod)(const char *path, fuse_mode_t mode, fuse_dev_t dev);
    int (*mkdir)(const char *path, fuse_mode_t mode);
    int (*unlink)(const char *path);
    int (*rmdir)(const char *path);
    int (*symlink)(const char *dstpath, const char *srcpath);
    int (*rename)(const char *oldpath, const char *newpath, unsigned int flags);
    int (*link)(const char *srcpath, const char *dstpath);
    int (*chmod)(const char *path, fuse_mode_t mode);
    int (*chown)(const char *path, fuse_uid_t uid, fuse_gid_t gid);
    int (*truncate)(const char *path, fuse_off_t size);
    int (*open)(const char *path, struct fuse_file_info *fi);
    int (*read)(const char *path, char *buf, size_t size, fuse_off_t off,
        struct fuse_file_info *fi);
    int (*write)(const char *path, const char *buf, size_t size, fuse_off_t off,
        struct fuse_file_info *fi);
    int (*statfs)(const char *path, struct fuse_statvfs *stbuf);
    int (*flush)(const char *path, struct fuse_file_info *fi);
    int (*release)(const char *path, struct fuse_file_info *fi);
    int (*fsync)(const char *path, int datasync, struct fuse_file_info *fi);
    int (*setxattr)(const char *path, const char *name, const char *value, size_t size,
        int flags);
    int (*getxattr)(const char *path, const char *name, char *value, size_t size);
    int (*listxattr)(const char *path, char *namebuf, size_t size);
    int (*removexattr)(const char *path, const char *name);
    int (*opendir)(const char *path, struct fuse_file_info *fi);
    int (*readdir)(const char *path, void *buf, fuse_fill_dir_t filler, fuse_off_t off,
        struct fuse_file_info *fi, enum fuse_readdir_flags flags);
    int (*releasedir)(const char *path, struct fuse_file_info *fi);
    int (*fsyncdir)(const char *path, int datasync, struct fuse_file_info *fi);
    void *(*init)(struct fuse_conn_info *conn);
    void (*destroy)(void *data);
    int (*access)(const char *path, int mask);
    int (*create)(const char *path, fuse_mode_t mode, struct fuse_file_info *fi);
    int (*ftruncate)(const char *path, fuse_off_t off, struct fuse_file_info *fi);
    int (*fgetattr)(const char *path, struct fuse_stat *stbuf, struct fuse_file_info *fi);
    int (*lock)(const char *path, struct fuse_file_info *fi, int cmd, struct flock *lock);
    int (*utimens)(const char *path, const struct timespec tv[2]);
    int (*bmap)(const char *path, size_t blocksize, uint64_t *idx);
    int (*ioctl)(const char *path, int cmd, void *arg, struct fuse_file_info *fi,
        unsigned int flags, void *data);
    int (*poll)(const char *path, struct fuse_file_info *fi,
        struct fuse_pollhandle *ph, unsigned *reventsp);
    int (*write_buf)(const char *path, struct fuse_bufvec *buf, fuse_off_t off,
        struct fuse_file_info *fi);
    int (*read_buf)(const char *path, struct fuse_bufvec **bufp,
        size_t size, fuse_off_t off, struct fuse_file_info *fi);
    int (*flock)(const char *path, struct fuse_file_info *, int op);
    int (*fallocate)(const char *path, int mode, fuse_off_t off, fuse_off_t len,
        struct fuse_file_info *fi);
};

struct fuse_context
{
    struct fuse *fuse;
    fuse_uid_t uid;
    fuse_gid_t gid;
    fuse_pid_t pid;
    void *private_data;
    fuse_mode_t umask;
};

#define fuse_main(argc, argv, ops, data)\
    fuse_main_real(argc, argv, ops, sizeof *(ops), data)

FSP_FUSE_API int fsp_fuse_main_real(int argc, char *argv[],
    const struct fuse_operations *ops, size_t opsize, void *data,
    int environment);
FSP_FUSE_API struct fuse *fsp_fuse_new(struct fuse_chan *ch, struct fuse_args *args,
    const struct fuse_operations *ops, size_t opsize, void *data,
    int environment);
FSP_FUSE_API void fsp_fuse_destroy(struct fuse *f);
FSP_FUSE_API int fsp_fuse_loop(struct fuse *f);
FSP_FUSE_API int fsp_fuse_loop_mt(struct fuse *f);
FSP_FUSE_API void fsp_fuse_exit(struct fuse *f);
FSP_FUSE_API struct fuse_context *fsp_fuse_get_context(void);

static inline int fuse_main_real(int argc, char *argv[],
    const struct fuse_operations *ops, size_t opsize, void *data)
{
    return fsp_fuse_main_real(argc, argv, ops, opsize, data, FSP_FUSE_ENVIRONMENT);
}

static inline struct fuse *fuse_new(struct fuse_chan *ch, struct fuse_args *args,
    const struct fuse_operations *ops, size_t opsize, void *data)
{
    return fsp_fuse_new(ch, args, ops, opsize, data, FSP_FUSE_ENVIRONMENT);
}

static inline void fuse_destroy(struct fuse *f)
{
    fsp_fuse_destroy(f);
}

static inline int fuse_loop(struct fuse *f)
{
    return fuse_loop(f);
}

static inline int fuse_loop_mt(struct fuse *f)
{
    return fsp_fuse_loop_mt(f);
}

static inline void fuse_exit(struct fuse *f)
{
    fsp_fuse_exit(f);
}

static inline struct fuse_context *fuse_get_context(void)
{
    return fsp_fuse_get_context();
}

static inline int fuse_getgroups(int size, fuse_gid_t list[])
{
    return 0;
}

static inline int fuse_interrupted(void)
{
    return 0;
}

static inline int fuse_start_cleanup_thread(struct fuse *fuse)
{
    return 0;
}

static inline void fuse_stop_cleanup_thread(struct fuse *fuse)
{
}

static inline int fuse_clean_cache(struct fuse *fuse)
{
    return 60;
}

static inline struct fuse_session *fuse_get_session(struct fuse *f)
{
    return (void *)f;
}

#ifdef __cplusplus
}
#endif

#endif