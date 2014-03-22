/*
 * Timer.cpp:
 *  Simple timing utility that uses either the Windows or Posix API. Thus handles vast majority of cases.
 *  Public domain (by Adam Domurad)
 */

#include <cstdlib>

#include "Timer.h"

#ifdef _WIN32
#include <windows.h>
const long long DELTA_EPOCH_IN_MICROSECS= 11644473600000000LL;

#if !defined(_TIMEVAL_DEFINED) && !defined(_MSC_VER)
struct timeval {
	int tv_sec; /* seconds */
	int tv_usec; /* microseconds */
};
#endif

int gettimeofday(struct timeval *tv, void* notused) {
	FILETIME ft;
	long long tmpres = 0;

	memset(&ft, 0, sizeof(ft));

	GetSystemTimeAsFileTime(&ft);

	tmpres = ft.dwHighDateTime;
	tmpres <<= 32;
	tmpres |= ft.dwLowDateTime;

	/*converting file time to unix epoch*/
	tmpres /= 10; /*convert into microseconds*/
	tmpres -= DELTA_EPOCH_IN_MICROSECS;
	tv->tv_sec = (int)(tmpres*0.000001);
	tv->tv_usec =(tmpres%1000000);

	return 0;
}
#else
#include <sys/time.h>
#endif

Timer::Timer() {
	start();
}

void Timer::start() {
	timeval timev;
	gettimeofday(&timev, NULL);
	microseconds_since_epoch = timev.tv_sec * 1000 * 1000 + timev.tv_usec;
}

unsigned long long Timer::get_microseconds() const {
	Timer now;
	return now.microseconds_since_epoch - microseconds_since_epoch;
}
