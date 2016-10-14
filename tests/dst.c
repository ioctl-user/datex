#include <stdio.h>
#include <unistd.h>

#include "datex.h"
#include "datex_type.h"

int main()
{
	int i;
	datex_d *ddesc;
	d3_t d;
	
	if ((ddesc = datex_open("d3", sizeof(d), DATEX_READ)) == NULL) {
		return 1;
	}

	for (i = 0;;i++) {
		datex_read(ddesc, &d);
#if 0
		if (i % 10000000 == 0) {
			printf("I %d R %d\n", i, d.d[0]);
			if (i == 1000000000)
				return 0;
		}
#endif
#if 1
		printf("%d %d %d\n", d.d[0], d.d[1], d.d[2]);
		usleep(10000);
#endif
	}

	datex_close(ddesc);
}
