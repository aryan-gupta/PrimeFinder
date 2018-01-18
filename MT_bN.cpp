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
 * @title Prime Finder by Indidual Number
 * @date 2017-02-12 (YYYY-MM-DD)
 * @fversion 2.3.1
 * @description Finds prime numbers by multithreading using single numbers
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
#include <atomic>

#include ".\h\global.h"
#include ".\h\MT_bN.h"

using ull = unsigned long long;

namespace MThreaded::byNumber {

std::mutex found;
std::vector<ull> foundPrimes;
std::atomic<ull> lastTested;

bool testForPrime(const ull num) {
	for(ull i = 2; i < num / 2 + 1; ++i) {
		if(num % i == 0) {
			return false;
		}
	}
	return true;
}

void lookForPrimes() {
	while(true) {
		ull num = lastTested.fetch_add(1, std::memory_order_relaxed);
		
		if(num > END)
			break;
		
		if(testForPrime(num)) {
			std::scoped_lock lk{ found }
			foundPrimes.push_back(num);
		}
	}
}

void displayPrimes() {
	std::cout << std::endl;
	for(auto& v: foundPrimes) {
		std::cout << v << "\t";
	}
}

ReturnData run() {
	using namespace std::chrono;
	using namespace std;
	
	vector<thread> threads; // vector storing threads
	foundPrimes.reserve(END);
	
	auto startTime = steady_clock::now();
	for(int i = 0; i < NUM_OF_THREADS; ++i)
		threads.emplace_back(lookForPrimes);

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
	data.primesFound = foundPrimes.size();
	data.time = nseconds;
	
	return data;
}


} // end namespace MThreaded::byNumber
