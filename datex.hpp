/* By Aleksey Kazantsev */
#ifndef _LIBDATA_HPP_
#define _LIBDATA_HPP_

#ifdef __cplusplus
extern "C" {
#endif

#include <datex.h>
#include <datex_type.h>

#ifdef __cplusplus
} /* extern C */
#endif

/* Datex class definition */
struct dxp {
	datex_d *ddesc;
};

template <class dxt> class datex: private dxp, public dxt {
public:
	datex(const char *fname, unsigned short flags) {
		ddesc = datex_open(fname, sizeof(dxt), flags);
	};
	void wait() {
		datex_wait(this->ddesc);
	};
	void read() {
		datex_read(this->ddesc, static_cast<dxt*>(this));
	};
	void write() {
		datex_write(this->ddesc, static_cast<dxt*>(this));
	};
	~datex() {
		datex_close(this->ddesc);
	};
};

#endif
