/*
 * Timeouts.cpp
 *
 * Created: 16.02.2017 12:19:37
 *  Author: Dima
 */ 

#include "Timeouts.h"

// global seconds counter
uint Seconds(0);

uchar cTimeoutMs::operator()(){
	uint CurT = TCNT1, dt; //7812 Hz clock
	if(CurT < PrevT)
	dt = 65535 - PrevT + CurT;
	else
	dt = CurT - PrevT;
	if(dt >= Period){
		PrevT = CurT;
		dt -= Period;
		if(dt && dt < Period) PrevT -= dt;
		return 1;
	}
	return 0;
}

void cTimeoutMs::Reset(){
	PrevT = TCNT1;
}

void cTimeoutMs::SetPeriod(const uint ms){
	Period = uint32_t(ms) * 7812 / 1000;
	Reset();
}

//===========================================

// check for time is out and reload automatically
uchar cTimeoutSec::operator()(){
	uint CurT = Seconds, dt;
	if(CurT < PrevT)
	dt = 65535 - PrevT + CurT;
	else
	dt = CurT - PrevT;
	if(dt >= Period){
		PrevT = CurT;
		dt -= Period;
		if(dt && dt < Period) PrevT -= dt;
		return 1;
	}
	return 0;
}

void cTimeoutSec::Reset(){
	PrevT = Seconds;
}

void cTimeoutSec::SetPeriod(const uint sec){
	Period = sec;
	Reset();
}