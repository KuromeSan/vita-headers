/**
 * \kernelgroup{SceIofilemgr}
 * \usage{psp2kern/kernel/iofilemgr.h,SceIofilemgrForDriver_stub}
 */

#ifndef _DOLCESDK_PSP2KERN_KERNEL_IOFILEMGR_H_
#define _DOLCESDK_PSP2KERN_KERNEL_IOFILEMGR_H_

#include <psp2kern/kernel/types.h>
#include <psp2kern/kernel/iofilemgr/stat.h>
#include <psp2kern/kernel/iofilemgr/dirent.h>

#ifdef __cplusplus
extern "C" {
#endif

/* device types */
#define SCE_D_TYPE_NULL     0x0     /* dummy device */
#define SCE_D_TYPE_CHAR     0x1     /* character device */
#define SCE_D_TYPE_BLOCK    0x4     /* block device */
#define SCE_D_TYPE_FS       0x10    /* file system */
#define SCE_D_TYPE_ALIAS    0x20    /* alias name (device alias) */
#define SCE_D_TYPE_MOUNTPT  0x40    /* alias name (mount point) */

/* flags */
#define SCE_FREAD       (0x0001)        /* readable */
#define SCE_FWRITE      (0x0002)        /* writable */
#define SCE_FNBLOCK     (0x0004)        /* Reserved: non-blocking reads */
#define SCE_FDIRO       (0x0008)        /* internal use for dopen */
#define SCE_FRLOCK      (0x0010)        /* Reserved: read locked (non-shared) */
#define SCE_FWLOCK      (0x0020)        /* Reserved: write locked (non-shared) */
#define SCE_FAPPEND     (0x0100)        /* append on each write */
#define SCE_FCREAT      (0x0200)        /* create if nonexistant */
#define SCE_FTRUNC      (0x0400)        /* truncate to zero length */
#define SCE_EXCL        (0x0800)        /* exclusive create */
#define SCE_FSCAN       (0x1000)        /* Reserved: scan type */
#define SCE_FRCOM       (0x2000)        /* Reserved: remote command entry */
#define SCE_FNBUF       (0x4000)        /* no device buffer */
#define SCE_FASYNC      (0x8000)        /* Reserved: asyncronous i/o */
#define SCE_FFDEXCL     (0x01000000)    /* exclusive access */
#define SCE_FGAMEDATA   (0x40000000)

/* Flag for ksceIoOpen() */
#define SCE_O_RDONLY    (SCE_FREAD)
#define SCE_O_WRONLY    (SCE_FWRITE)
#define SCE_O_RDWR      (SCE_FREAD|SCE_FWRITE)
#define SCE_O_NBLOCK    (SCE_FNBLOCK) /* Reserved: Non-Blocking I/O */
#define SCE_O_APPEND    (SCE_FAPPEND) /* append (writes guaranteed at the end) */
#define SCE_O_CREAT     (SCE_FCREAT)  /* open with file create */
#define SCE_O_TRUNC     (SCE_FTRUNC)  /* open with truncation */
#define SCE_O_EXCL      (SCE_EXCL)    /* exclusive create */
#define SCE_O_NOBUF     (SCE_FNBUF)   /* no device buffer */
#define SCE_O_NOWAIT    (SCE_FASYNC)  /* Reserved: asyncronous i/o */
#define SCE_O_FDEXCL    (SCE_FFDEXCL) /* exclusive access */
#define SCE_O_PWLOCK    (SCE_FPWLOCK) /* power control lock */
#define SCE_O_FGAMEDATA (SCE_FGAMEDATA)

/* Flag for ksceIoLseek() */
#define SCE_SEEK_SET	(0)
#define SCE_SEEK_CUR	(1)
#define SCE_SEEK_END	(2)

/* Path */
#define SCE_IO_MAX_PATH_BUFFER_SIZE  (1024) /* maximum path buffer size */
#define SCE_IO_MAX_PATH_LENGTH       (200)  /* maximum path length */

/**
 * May be used with ksceIoDevctl()
 */
typedef struct SceIoDevInfo {
  SceOff max_size;
  SceOff free_size;
  SceSize cluster_size;
  void *unk;
} SceIoDevInfo;

/**
 * Remove directory entry
 *
 * @param filename - Path to the file to remove
 * @return < 0 on error
 */
int ksceIoRemove(const char *filename);

/**
 * Make a directory file
 *
 * @param dirname - The path to the directory
 * @param mode - Access mode bits.
 * @return Returns the value 0 if it's successful, otherwise <0
 */
int ksceIoMkdir(const char *dirname, SceIoMode mode);

/**
 * Remove a directory file
 *
 * @param dirname - Removes a directory file pointed by the string path
 * @return Returns the value 0 if it's successful, otherwise <0
 */
int ksceIoRmdir(const char *dirname);

/**
 * Change the name of a file
 *
 * @param oldname - The old filename
 * @param newname - The new filename
 * @return < 0 on error.
 */
int ksceIoRename(const char *oldname, const char *newname);

/**
  * Send a devctl command to a device.
  *
  * @par Example: Sending a simple command to a device
  * @code
  * SceIoDevInfo info;
  * ksceIoDevctl("ux0:", 0x3001, NULL, 0, &info, sizeof(SceIoDevInfo));
  * @endcode
  *
  * @param devname - String for the device to send the devctl to (e.g. "ux0:")
  * @param cmd - The command to send to the device
  * @param arg - A data block to send to the device, if NULL sends no data
  * @param arglen - Length of indata, if 0 sends no data
  * @param bufp - A data block to receive the result of a command, if NULL receives no data
  * @param buflen - Length of outdata, if 0 receives no data
  * @return 0 on success, < 0 on error
  */
int ksceIoDevctl(
	const char *devname,
	int cmd,
	const void *arg,
	SceSize arglen,
	void *bufp,
	SceSize buflen);

/**
  * Synchronize the file data on the device.
  *
  * @param devname - The device to synchronize (e.g. msfat0:)
  * @param flag - device specific flags
  */
int ksceIoSync(const char *devname, int flag);

/**
 * Open or create a file for reading or writing
 *
 * @par Example1: Open a file for reading
 * @code
 * if((fd = ksceIoOpen("device:/path/to/file", SCE_O_RDONLY, 0777) < 0) {
 * // error code in fd, for example no open filehandle left (0x80010018)
 * }
 * @endcode
 * @par Example2: Open a file for writing, creating it if it doesn't exist
 * @code
 * if((fd = ksceIoOpen("device:/path/to/file", SCE_O_WRONLY|SCE_O_CREAT, 0777) < 0) {
 * // error code in fd, for example no open filehandle left (0x80010018)
 * }
 * @endcode
 *
 * @param filename - Pointer to a string holding the name of the file to open
 * @param flag - Libc styled flags that are or'ed together
 * @param mode - File access mode (One or more ::SceIoMode).
 * @return A non-negative integer is a valid fd, anything else an error
 */
SceUID ksceIoOpen(const char *filename, int flag, SceIoMode mode);

/**
 * Delete a descriptor
 *
 * @code
 * ksceIoClose(fd);
 * @endcode
 *
 * @param fd - File descriptor to close
 * @return < 0 on error
 */
int ksceIoClose(SceUID fd);

/**
 * Reposition read/write file descriptor offset
 *
 * @par Example:
 * @code
 * pos = ksceIoLseek(fd, -10, SCE_SEEK_END);
 * @endcode
 *
 * @param fd - Opened file descriptor with which to seek
 * @param offset - Relative offset from the start position given by whence
 * @param whence - One of ::SceIoSeekMode.
 *
 * @return The position in the file after the seek.
 */
SceOff ksceIoLseek(SceUID fd, SceOff offset, int whence);

/**
 * Read input
 *
 * @par Example:
 * @code
 * bytes_read = ksceIoRead(fd, data, 100);
 * @endcode
 *
 * @param fd - Opened file descriptor to read from
 * @param buf - Pointer to the buffer where the read data will be placed
 * @param nbyte - Size of the read in bytes
 *
 * @return The number of bytes read
 */
SceSSize ksceIoRead(SceUID fd, void *buf, SceSize nbyte);

/**
 * Write output
 *
 * @par Example:
 * @code
 * bytes_written = ksceIoWrite(fd, data, 100);
 * @endcode
 *
 * @param fd - Opened file descriptor to write to
 * @param buf - Pointer to the data to write
 * @param nbyte - Size of data to write
 *
 * @return The number of bytes written
 */
SceSSize ksceIoWrite(SceUID fd, const void *buf, SceSize nbyte);

/**
 * Read input at offset
 *
 * @par Example:
 * @code
 * bytes_read = ksceIoPread(fd, data, 100, 0x1000);
 * @endcode
 *
 * @param fd - Opened file descriptor to read from
 * @param buf - Pointer to the buffer where the read data will be placed
 * @param nbyte - Size of the read in bytes
 * @param offset - Offset to read
 *
 * @return < 0 on error.
 */
SceSSize ksceIoPread(SceUID fd, void *buf, SceSize nbyte, SceOff offset);

/**
 * Write output at offset
 *
 * @par Example:
 * @code
 * bytes_written = ksceIoPwrite(fd, data, 100, 0x1000);
 * @endcode
 *
 * @param fd - Opened file descriptor to write to
 * @param buf - Pointer to the data to write
 * @param nbyte - Size of data to write
 * @param offset - Offset to write
 *
 * @return The number of bytes written
 */
SceSSize ksceIoPwrite(SceUID fd, const void *buf, SceSize nbyte, SceOff offset);

/**
 * Synchronize the file data for one file
 *
 * @param fd - Opened file descriptor to sync
 * @param flag - device specific flags
 *
 * @return < 0 on error.
 */
int ksceIoSyncByFd(SceUID fd, int flag);

/*--------------------Async IO--------------------*/

/**
  * This function is unimplemented.
  *
  * @return SCE_KERNEL_ERROR_UNSUP (0x80020004)
  */
int ksceIoIoctlAsync(
  SceUID fd,
  int cmd,
  const void *argp,
  SceSize arglen,
  void *bufp,
  SceSize buflen,
  SceIoAsyncParam* asyncParam);

/**
  * This function is unimplemented.
  *
  * @return SCE_KERNEL_ERROR_UNSUP (0x80020004)
  */
int ksceIoDevctlAsync(
  const char *devname,
  int cmd,
  const void *arg,
  SceSize arglen,
  void *bufp,
  SceSize buflen,
  SceIoAsyncParam* asyncParam);

/*--------------------Device mount functions--------------------*/

/**
  * Mounts a device
  *
  * @param[in]  id          Device to mount
  * @param[in]  path        Where to mount to
  * @param[in]  permission  Permission flags
  * @param      a4          Unknown, set to 0
  * @param      a5          Unknown, set to 0
  * @param      a6          Unknown, set to 0
  *
  * @return < 0 on error.
  */
int ksceIoMount(int id, const char *path, int permission, int a4, int a5, int a6);

/**
  * Unmounts a device
  *
  * @param[in]  id    Device to unmount
  * @param[in]  a2    Unknown, set to 0
  * @param[in]  a3    Unknown, set to 0
  * @param[in]  a4    Unknown, set to 0
  *
  * @return < 0 on error.
  */
int ksceIoUmount(int id, int a2, int a3, int a4);

#ifdef __cplusplus
}
#endif

#endif	/* _DOLCESDK_PSP2KERN_KERNEL_IOFILEMGR_H_ */
