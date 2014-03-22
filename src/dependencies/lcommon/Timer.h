/*
 * Timer.h:
 *  Simple timing utility.
 *  Public domain (by Adam Domurad)
 */

#ifndef LCOMMON_TIMER_H_
#define LCOMMON_TIMER_H_

class Timer {
public:
	Timer();
	void start();
	unsigned long long get_microseconds() const;
private:
	long long microseconds_since_epoch;
};

#endif /* LCOMMON_TIMER_H_ */
