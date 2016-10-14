#include <iostream>
#include "datex.hpp"
#include "datex_type.hpp"

int main(void)
{
	int i;
	datex<d3_t> d3 ("d3", DATEX_READ | DATEX_WAIT);

	for (i = 0;;i++) {
		d3.wait();
		d3.read();
		std::cout << d3.d[0] << " " << d3.d[1] << " " << d3.d[2] << "\n";
	}
}
