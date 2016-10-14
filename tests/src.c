#include <stdio.h>

#include "datex.h"
#include "datex_type.h"

int main()
{
	int i;
	datex_d *ddesc;
	d3_t d;

	if ((ddesc = datex_open("d3", sizeof(d), DATEX_WRITE)) == NULL) {
		return 1;
	}

	for (i = 0;;i++) {
		d.d[0] = i;
		d.d[1] = i;
		d.d[2] = i;

		datex_write(ddesc, &d);

		if (i % 10000000 == 0) {
			printf("%d\n", i);
#if 1
			if (i == 1000000000)
				return 0;
#endif
		}
	}

	datex_close(ddesc);
}
