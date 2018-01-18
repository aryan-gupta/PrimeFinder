/* 
 * Copyright (c) The Gupta Empire - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Aryan Gupta <me@theguptaempire.net>
 * 
 * ====================================================================================
 * @author Aryan Gupta
 * @project Prime Finder
 * @title Prime Finder Using Multiple Ways
 * @date 2017-02-12 (YYYY-MM-DD)
 * @fversion 2.1.1
 * @description Finds prime numbers by multithreading using sections
 * ====================================================================================
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <chrono>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <condition_variable>

#include ".\h\global.h"
#include ".\h\MT_bS.h"

typedef unsigned long long ull;

namespace MThreaded::bySection {
	
std::mutex mtx;
std::vector<std::vector<ull>> foundPrimes;

bool testForPrime(const ull num) {
	for(ull i = 2; i < num / 2 + 1; ++i) {
		if(num % i == 0) {
			return false;
		}
	}
	return true;
}

void lookForPrimes(const ull start, const ull end) {
	std::vector<ull>foundPrimes_local;
	foundPrimes_local.reserve(end - start + 100);
	
	for(ull i = start; i <= end; ++i) {
		if(testForPrime(i)) {
			foundPrimes_local.push_back(i);
		}
	}
	
	std::scoped_lock lk{ mtx };
	foundPrimes.emplace_back(std::move(foundPrimes_local));
}

void displayPrimes() {
	std::cout << std::endl;
	for(auto& v: foundPrimes) {
		for(auto i: v)
			std::cout << i << "\t";
	}
}

ReturnData run() {
	using namespace std;
	using namespace std::chrono;

	vector<thread> threads; // vector storing threads
	
	ull distance = ((END - START) / NUM_OF_THREADS);
	
	/** 
		divide the whole section where to look for primes
		in to number of threads. then multiply that with
		the thread to find the end values
		EX.
		start = 3
		end = 100
		num of threads = 3
		end - start = 97
		97 / 3 = 33.333333 = 33
		section 1 = start + distance * 1 = 3  to 33
		section 2 = start + distance * 2 = 34 to 66
		section 3 = (end 2 + 1) to end   = 67 to 100
	 **/
	auto startTime = steady_clock::now();
	
	for(int i = 0; i < NUM_OF_THREADS - 1; ++i)
		threads.emplace_back(lookForPrimes, START + (distance * (i)), START + (distance * (i + 1)));
	threads.emplace_back(lookForPrimes, START + (distance * (NUM_OF_THREADS - 1)), END);

	for(auto& t: threads) t.join();
	
	auto endTime = steady_clock::now();
	auto diff = endTime - startTime;
	auto nseconds = duration_cast<milli>(diff).count();
	
	if(dAll) {
		cout << "Primes: ";
		displayPrimes();
		cout << endl;
	}
	
	ReturnData data;
	
	for(auto& v: foundPrimes)
		data.primesFound += v.size();;
		
	data.time = nseconds;
	
	return data;
}
	
} // end namespace MThreaded::bySection
