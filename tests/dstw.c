#include <stdio.h>

#include "datex.h"
#include "datex_type.h"

int main()
{
	int i;
	datex_d *ddesc;
	d3_t d;

	if ((ddesc = datex_open("d3", sizeof(d), DATEX_READ|DATEX_WAIT)) == NULL) {
		return 1;
	}

	for (i = 0;;i++) {
		datex_wait(ddesc);
		datex_read(ddesc, &d);
		printf("%d %d %d\n", d.d[0], d.d[1], d.d[2]);
	}

	datex_close(ddesc);
}
