#ifndef _LIBDATA_TYPE_H_
#define _LIBDATA_TYPE_H_

typedef struct {
	int d[3];
} d3_t;

typedef struct {
	struct {
		unsigned char arr[10000];
		long ttt;
	} foo;
	int bar;
} bigarea_t;

#endif
