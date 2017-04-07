/* 
 * Copyright (c) The Gupta Empire - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Aryan Gupta <me@theguptaempire.net>, February 2017
 * 
 * =============================================================================
 * @author Aryan Gupta
 * @project Prime Finder
 * @title Single Threaded Finder
 * @date 2017-02-11 (YYYY-MM-DD)
 * @fversion 2.1.0
 * @description Finds prime numbers using one thread
 * =============================================================================
 */
 
#include <cstdlib>
#include <iostream>
#include <vector>
#include <chrono>
#include <stdlib.h>

#if (defined (LINUX) || defined (__linux__))
	#include <thread>
	#include <mutex>
	#include <condition_variable>
#elif (defined (_WIN32) || defined (_WIN64))
	#include ".\h\mingw.thread.h"
	#include ".\h\mingw.mutex.h"
#endif

#include ".\h\global.h"

namespace SThreaded {
	std::vector<long> foundPrimes;

	bool testForPrime(const unsigned int num) {
		for(unsigned int i = 2; i < num; ++i) {
			if(num % i == 0) {
				return false;
			}
		}
		return true;
	}

	void lookForPrimes(const unsigned int start, const unsigned int end) {		
		for(unsigned int i = start; i <= end; i++)
			if(testForPrime(i))
				foundPrimes.push_back(i);
	}

	void displayPrimes() {
		for(auto& v: foundPrimes)
			std::cout << v << "\t";
	}

	ReturnData run() {
		using namespace std::chrono;
		using std::cout;
		using std::endl;
		
		foundPrimes.reserve(END + 100);
		
		auto start = steady_clock::now();
		lookForPrimes(START, END);
		auto end = steady_clock::now();
		
		
		auto diff = end - start;
		auto nseconds = double(diff.count()) * steady_clock::period::num / steady_clock::period::den;
		
		if(dAll) {
			cout << "Primes: ";
			displayPrimes();
			cout << endl;
		}
		
		ReturnData data;
		data.primesFound = foundPrimes.size();
		data.time = nseconds;
		
		return data;
	}

}


