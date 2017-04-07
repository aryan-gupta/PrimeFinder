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

#if (defined (LINUX) || defined (__linux__))
	#include <thread>
	#include <mutex>
	#include <condition_variable>
#elif (defined (_WIN32) || defined (_WIN64))
	#include ".\h\mingw.thread.h"
	#include ".\h\mingw.mutex.h"
#endif

#include ".\h\global.h"
#include ".\h\MT_bN.h"

typedef unsigned long long ull;

namespace MThreaded {
	namespace byNumber {
		std::mutex testing;
		std::mutex found;
		std::vector<ull> foundPrimes;
		std::vector<ull> testedPrimes(1, START - 1);

		bool testForPrime(const ull num) {
			for(ull i = 2; i < num; ++i) {
				if(num % i == 0) {
					return false;
				}
			}
			return true;
		}

		void lookForPrimes() {
			while(true) {
				testing.lock();
				ull num = testedPrimes.back() + 1;
				testedPrimes.push_back(num);
				testing.unlock();
				
				if(num > END)
					break;
				
				if(testForPrime(num)) {
					found.lock();
					foundPrimes.push_back(num);
					found.unlock();
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
			
			threads.reserve(NUM_OF_THREADS);
			foundPrimes.reserve(END + 100);
			
			auto startTime = steady_clock::now();
			for(int i = 0; i < NUM_OF_THREADS; ++i)
				threads.push_back(thread(lookForPrimes));

			for(auto& t: threads) t.join();
			
			auto endTime = steady_clock::now();
			auto diff = endTime - startTime;
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
}
