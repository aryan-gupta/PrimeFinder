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
 * @title Prime Finder Next Prime
 * @date 2017-02-17 (YYYY-MM-DD)
 * @fversion 2.1.3
 * @description Finds next prime number after a number
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

typedef unsigned long long ull;

namespace MThreaded::NextPrime {

using std::vector;
using std::cout;
using std::endl;
using std::thread;
using std::mutex;

constexpr bool DEBUG = true;

std::atomic_bool stopAllThreads = false;
std::atomic_bool foundNextPrime = false;



ull numTesting;
vector<bool> threadFinished(NUM_OF_THREADS, false);
vector<bool> isPotentialPrime(NUM_OF_THREADS, true);

std::atomic_flag calcDone = ATOMIC_FLAG_INIT;

std::condition_variable continueWork;
mutex cw_lock;

std::condition_variable startWork;
mutex sw_lock;
std::atomic_int waiting;

mutex mtx_cout;

std::pair<bool, bool> testForPrime(const ull start, const ull end, std::atomic_bool& stop) {
	// First return value is if its a prime in that interval, second is if it was a premature
	// stop, Dont know why we need it but, hey....
	for(unsigned int i = start; i < end; ++i) {
		if (stop.load(std::memory_order_relaxed)) return {false, true};
		
		if(i < 2) i = 2;
		
		if(numTesting % i == 0) {
			stop.store(true, std::memory_order_relaxed);
			return {false, false};
		}
	}
	return {true, false};
}

template <typename T>
bool isArraySame(const T& arr, const bool val) {
	for(auto b : arr) {
		if (b != val)
			return false;
	}
	return true;
}

void resetThread(int id) {
	threadFinished[id] = false;
	isPotentialPrime[id] = false;
}

void threadPool(int id) {
	while(true) {		
		std::unique_lock lk{ sw_lock };
		resetThread(id);
		// This waits until all the threads have set their bool flag to false. 
		// The first time this is run, this has no effect because the flag is already
		// false, however after the first run it will work as a barier, that waits
		// for all the threads to reach here ('starting point'). There is another wait
		// at the end that guarentees that all the flags are set as true. Also this does
		// not set 
		startWork.wait(lk, [&]{ return isArraySame(threadFinished, false); });
		startWork.notify_all();
		lk.unlock();
		
		long distance = numTesting / NUM_OF_THREADS;
		
		if constexpr (DEBUG) {
			mtx_cout.lock();
			cout << "ID: " << id << " testing " << numTesting << " from " << distance * id << " to " << distance * (id + 1) << endl;
			mtx_cout.unlock();
		}
		
		auto [PP, PS] = testForPrime(distance * id, distance * (id + 1), stopAllThreads);
		
		isPotentialPrime[id] = PP; // each thread is accessing different locations, its fine
		
		if constexpr (DEBUG) {
			mtx_cout.lock();
			cout << "ID: " << id << " calculated " << numTesting << (isPotentialPrime[id]? " is not a prime":" could be a prime") << endl;
			mtx_cout.unlock();
		}
		
		threadFinished[id] = true;
		
		if constexpr (DEBUG) {
			mtx_cout.lock();
			cout << "ID: " << id << " waiting for other threads to finish " << endl;
			mtx_cout.unlock();
		}	
		
		std::unique_lock lk(test);
		continueWork.wait(lk, [&]{ return isArraySame(threadFinished, true); });
		lk.unlock();
		continueWork.notify_all();
		
		if (!calcDone.test_and_set(std::memory_order_relaxed)) {
			if (PS) {
				numTesting++;
			} else {
				if (isArraySame(isPotentialPrime, true))
					
			}
		}
	}
}

ReturnData run(ull tester) {
	using namespace std::chrono;
	using namespace std;
	
	vector<thread> threads; // vector storing threads
	
	numTesting = tester;
	
	auto startTime = steady_clock::now();
	
	for(int i = 0; i < NUM_OF_THREADS; ++i)
		threads.emplace_back(threadPool, i);

	for(auto& t: threads) t.join();
	
	auto endTime = steady_clock::now();
	auto diff = endTime - startTime;
	auto nseconds = duration_cast<milli>(diff).count();
	
	if(dAll) {
		cout << "Primes: ";
		//displayPrimes();
		cout << endl;
	}
	
	ReturnData data;
	data.nextPrime = numTesting;
	data.time = nseconds;
	
	return data;
}

} // end namespace MThreaded::NextPrime