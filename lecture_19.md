# 4/14/26 - Lecture 19

### read()/write() work with bytes
- In C, byte=char
- Not so for other types: int is often 4 bytes
- Require s care with non-char types
- All calls read/write actual bytes

### Standard File Descriptors
- When a process is born, comes with 3 open file descriptors
- Related to FILE* streams

| Symbol | # | FILE* | FD for... |
| -------- | ------- | ---------- | --------- |
| STDIN_FILENO | 0 | stdin | standard input (keyboard) |
| STDOUT_FILENO | 1 | stdout | standard output (screen) |
| STDERR_FILENO | 2 | stderr | standard error (screen) |


### Shell I/O Redirection
- Shells can direct input / output for programs using < and >

```bash
some_program > output.txt
# output in output.txt

interactive_prog < input.txt
# input.txt rather than typing

some_program &>> everything.txt
# stdout and stderr into everything.txt

some_program 2> /dev/null
# no stderr
```

### Manipulating the File Descriptor Table
- System calls `dup()` and `dup2()` manipulate the FD table
- `int backup_fd = dup(fd);` copies a file descriptor
- `dup2(src, dst);` copies fd at src to dst
(See picture)

### C `FILE` structs use file descirptors in UNIX
Typical Unix implemenation of standard I/O library `FILE` is
- A file descriptor
- Some buffers with positions
- Some options controlling buffering

For example, the FILE struct on Darwin/XNU
```c
typedef	struct __sFILE {
	unsigned char *_LIBC_UNSAFE_INDEXABLE	_p;	/* current position in (some) buffer */
	int	_r;		/* read space left for getc() */
	int	_w;		/* write space left for putc() */
	short	_flags;		/* flags, below; this FILE is free if 0 */
	short	_file;		/* fileno, if Unix descriptor, else -1 */
	struct	__sbuf _bf;	/* the buffer (at least 1 byte, if !NULL) */
	int	_lbfsize;	/* 0 or -_bf._size, for inline putc */

	/* operations */
	void	*_cookie;	/* cookie passed to io functions */
	int	(* _Nullable _close)(void *);
	int	(* _Nullable _read) (void *, char *_LIBC_COUNT(__n), int __n);
	fpos_t	(* _Nullable _seek) (void *, fpos_t, int);
	int	(* _Nullable _write)(void *, const char *_LIBC_COUNT(__n), int __n);

	/* separate buffer for long sequences of ungetc() */
	struct	__sbuf _ub;	/* ungetc buffer */
	struct __sFILEX *_extra; /* additions to FILE to not break ABI */
	int	_ur;		/* saved _r when _r is counting ungetc data */

	/* tricks to meet minimum requirements even when malloc() fails */
	unsigned char _ubuf[3];	/* guarantee an ungetc() buffer */
	unsigned char _nbuf[1];	/* guarantee a getc() buffer */

	/* separate buffer for fgetln() when line crosses buffer boundary */
	struct	__sbuf _lb;	/* buffer for fgetln() */

	/* Unix stdio files get aligned to block boundaries on fseek() */
	int	_blksize;	/* stat.st_blksize (may be != _bf._size) */
	fpos_t	_offset;	/* current lseek offset (see WARNING) */
} FILE;
```

### Mixing standard and low level I/O
- Standard IO (e.g. `printf`, `scanf`, `fprintf`, `fscanf`), is internally buffered by default
- Using a `read/write` call after may read/write in unexpected order

### Basic File Stats with `stat`

| Command | C functon | Effect |
| --------------- | --------------- | --------------- |
| stat -L file | `int ret = stat(file, &statbuf)` | Get statistics on file |
| stat file | `int ret = lstat(file, &statbuf)` | Don't follow symlinks |
| | |
| | `FILE fd = open(file, ...)` | Same as above but |
| | `int ret = fd(file, &statbuf)` | On a open file descriptor |


### Measuring Time in Code
- Measure CPU time with the standard `clock()` cunction; measure time difference and convert to seconds
- Meaasure Wall (real) time with `gettimeofday()` or related functions; fill struct with info on time of day

> [!NOTE]
> Wall and CPU time can diverge with scheduling, if other programs have significant competition with your program, Wall can be a lot longer than CPU time
