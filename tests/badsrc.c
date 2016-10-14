#include <stdio.h>

#include "datex.h"
#include "datex_type.h"

int main()
{
	int i;
	datex_d *ddesc;
	d3_t d;

	/* Incorrect initialization: do not request WRITE permissions */
	if ((ddesc = datex_open("d3", sizeof(d), DATEX_READ)) == NULL) {
		return 0;
	}

	for (i = 0;;i++) {
		d.d[0] = i;
		d.d[1] = i;
		d.d[2] = i;

		datex_write(ddesc, &d);

		if (i % 1000000 == 0)
			printf("%d\n", i);
	}

	datex_close(ddesc);
}
