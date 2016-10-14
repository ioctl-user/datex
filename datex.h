/* By Aleksey Kazantsev */
#ifndef _LIBDATA_H_
#define _LIBDATA_H_

#include <pthread.h>
#include <sys/mman.h> 
#include <sys/stat.h>        /* For mode constants */ 
#include <fcntl.h>           /* For O_* constants */ 

struct data_t;
typedef struct data_t data_t;
typedef unsigned int commit_t;

/* Type for local process datex area descriptor */
typedef struct {
	/* Use three different pointers to the same area
	instead of capability flags to avoid one flags check in fast
	read/write/wait function call for a better performance
	and more obvious error result -- segfault */
	data_t	*write;
	data_t	*read;
	data_t	*wait;
	commit_t commit;
} datex_d;

#define DATEX_READ  (1 << 0)
#define DATEX_WRITE (1 << 1)
#define DATEX_WAIT  (1 << 2)

datex_d *datex_open(const char *, unsigned int, unsigned short);
void datex_wait(datex_d *);
void datex_read(datex_d *, void *);
void datex_write(datex_d *, const void *);
void datex_close(datex_d *);

#endif
