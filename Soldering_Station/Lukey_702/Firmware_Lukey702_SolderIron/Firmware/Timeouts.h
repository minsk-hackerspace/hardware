/*
 * Timeouts.h
 *
 * Created: 16.02.2017 12:14:03
 *  Author: Dima
 */ 


#ifndef TIMEOUTS_H_
#define TIMEOUTS_H_

#include "Common.h"

// global seconds counter
extern uint Seconds;

// regular timeouts checker, resolution is 1 ms, maximum period is 8 sec
class cTimeoutMs{
	uint Period;
	uint PrevT;
	public:
	// period is in ms
	cTimeoutMs(const uint period){
		SetPeriod(period);
	}
	
	// check for time is out and reload automatically
	uchar operator()();	
	void Reset();	
	void SetPeriod(const uint ms);
};


// regular timeouts checker, resolution is 1 sec, maximum period is 18 hours
class cTimeoutSec{
	uint Period;
	uint PrevT;
	public:
	// period is in sec
	cTimeoutSec(const uint period){
		SetPeriod(period);
	}
	
	// check for time is out and reload automatically
	uchar operator()();	
	void Reset();	
	void SetPeriod(const uint sec);
};


#endif /* TIMEOUTS_H_ */