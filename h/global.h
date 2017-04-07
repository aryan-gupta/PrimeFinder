#ifndef _GLOBAL_H
#define _GLOBAL_H

extern int NUM_OF_THREADS;
extern bool dP;
extern bool dAll;
extern const int START;
extern unsigned long long END;

struct ReturnData {
	long primesFound = 0;
	double time = 0;
	bool isPrime = false;
	unsigned long long nextPrime = 0;
};

#endif