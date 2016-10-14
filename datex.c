/* By Aleksey Kazantsev */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* use POSIX shared memory */
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */

#include <fcntl.h>           /* For O_* constants */

#include <sys/syscall.h>
#include <linux/futex.h>
#include <limits.h>

#include <datex.h>

#define mb() asm volatile ("" ::: "memory")

/* Shared area header */
typedef struct {
	volatile commit_t commit;
	unsigned int flags;
	unsigned int psize;	/* payload size, expected to be set only once */
} serv;

/* Entire shared memory area data map */
struct data_t {
	serv s;
	void *p;	/* Used only to get address of this pointer */
};

datex_d *datex_open(const char *fname, unsigned int psize, unsigned short flags)
{
	int fd;
	datex_d *ddesc;
	unsigned int len;
	unsigned short mprot;
	unsigned int tmp;
	data_t *shm;

	ddesc = (datex_d *)calloc(1, sizeof(*ddesc));

	/* Calculate size to carry data_t with payload */
	len = psize * 2 + sizeof(serv);

	/* Map memory */
	fd = shm_open(fname, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		printf("Error while opening shared memory file %s\n", fname);
		free(ddesc);
		return NULL;
	}
	/* Set area lenght */
	if (ftruncate(fd, len)) {
		printf("Error while setting shared memory file %s lenght\n", fname);
		close(fd);
		free(ddesc);
		return(NULL);
	}
	/* Map */
	shm = (data_t *)mmap(0, len, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	close(fd);
	if (shm == MAP_FAILED) {
		printf("Error while mmaping shared memory file %s\n", fname);
		free(ddesc);
		return NULL;
	}

#if 1
	/* Write payload len to shared memory, if still not*/
	tmp = __sync_val_compare_and_swap(&shm->s.psize, 0, psize);
	if ((tmp != 0) && (shm->s.psize != psize)) {
		printf("Error: area %s payload len is %d, while should be %d\n",
				fname, shm->s.psize, psize);
		free(ddesc);
		return NULL;
	}
#else
	/* Write payload len to shared memory */
	shm->s.psize = psize;
#endif

	/* Set wait flag for shared memory area, if needed */
	if (flags & DATEX_WAIT)
		shm->s.flags |= DATEX_WAIT;

	/* Set requested pointers */
	if (flags & DATEX_WRITE)
		ddesc->write = shm;
	if (flags & DATEX_READ)
		ddesc->read = shm;
	if (flags & DATEX_WAIT)
		ddesc->wait = shm;

	/* Change area protection */
	mprot = 0;
	if (flags & DATEX_WRITE)
		mprot |= PROT_WRITE | PROT_READ;
	if (flags & (DATEX_READ | DATEX_WAIT))
		mprot |= PROT_READ;
	mprotect(shm, len, mprot);

	return ddesc;
}

void datex_wait(datex_d *ddesc)
{
	/* Make fast check in userspace */
	if (ddesc->commit != ddesc->wait->s.commit)
		return;
	syscall(__NR_futex, &ddesc->wait->s.commit, FUTEX_WAIT, ddesc->commit,NULL);
}

void datex_read(datex_d *ddesc, void *buf)
{
	short int read_from;

	do {
		ddesc->commit = ddesc->read->s.commit;
		read_from = ddesc->commit & 1;
		memcpy(buf, (void *)((unsigned long)&ddesc->read->p + read_from *
				ddesc->read->s.psize), ddesc->read->s.psize);
		mb();
	} while (ddesc->commit != ddesc->read->s.commit);
}

void datex_write(datex_d *ddesc, const void *buf)
{
	unsigned int write_to;

	write_to = (ddesc->write->s.commit & 1) ^ 1;
	memcpy((void *)((unsigned long)&ddesc->write->p + write_to *
			ddesc->write->s.psize), buf, ddesc->write->s.psize);
	mb();
	ddesc->write->s.commit++;

	if ((ddesc->write->s.flags & DATEX_WAIT) != 0)
		syscall(__NR_futex, &ddesc->write->s.commit, FUTEX_WAKE, INT_MAX);

}

void datex_close(datex_d *ddesc)
{
	data_t *shm;
	unsigned int psize;

	/* Find pointer to the area*/
	if (ddesc->read)
		shm = ddesc->read;
	else if (ddesc->write)
		shm = ddesc->write;
	else
		shm = ddesc->wait;

	psize = shm->s.psize;

	if (munmap(shm, psize * 2 + sizeof(serv))) {
		perror("Error while unmapping shared memory\n");
	}

	free(ddesc);
}

#ifdef __cplusplus
} /* extern C */
#endif
