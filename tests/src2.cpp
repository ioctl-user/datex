#include <iostream>
#include "datex.hpp"
#include "datex_type.hpp"

int main(void)
{
	int i;
	datex<d3_t> d3 ("d3", DATEX_WRITE);

	for (i = 0;;i++) {
		d3.d[0] = i;
		d3.d[1] = i;
		d3.d[2] = i;

		d3.write();

		if (i % 10000000 == 0) {
			std::cout << i << "\n";
#if 1
			if (i == 1000000000)
				return 0;
#endif
		}
	}
}
