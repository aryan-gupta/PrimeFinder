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
 * @fversion 2.3.0
 * @description Tests if a number is prime using multithreading
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
	#undef _GLIBCXX_HAS_GTHREADS
	#include ".\h\mingw.thread.h"
	#include <mutex>
	#include ".\h\mingw.mutex.h"
	#include ".\h\mingw.condition_variable.h"
	#include <atomic>
#endif

#include ".\h\global.h"
#include ".\h\MT_SN.h"

typedef unsigned long long ull;

namespace MThreaded {
	namespace SpecificNumber {
		using std::vector;
		using std::cout;
		using std::endl;
		using std::thread;
		using std::mutex;
		
		bool isPrime;
		ull numTesting;
		vector<bool> threadFinished(NUM_OF_THREADS, false);
		vector<bool> isPotentialPrime(NUM_OF_THREADS, true);
		
		std::condition_variable continueWork;
		
		mutex mtx_testIfFound;
		mutex mtx_finished;
		mutex mtx_cout;
		mutex mtx_isPP;
		
		bool testForPrime(const ull start, const ull end) {
			for(unsigned int i = start; i <= end; ++i) {
				if(i < 2) i = 2;
				if(numTesting % i == 0) {
					return false;
				}
			}
			return true;
		}
		
		bool otherThreadsFinished(/*int id*/) {
			int f = 0;
			//cout << "threadFinished ARRAY:" << threadFinished[0] << threadFinished[1] << threadFinished[2] << threadFinished[3] << endl;
			for(int i = 0; i < NUM_OF_THREADS; ++i)
				f += threadFinished[i];
			
			if(f == NUM_OF_THREADS)
				return true;
			return false;
		}
		
		void threadWork(int id) {
			ull distance = (numTesting / 2.0) / NUM_OF_THREADS;
			
			mtx_cout.lock();
			cout << "ID: " << id << " testing " << numTesting << " from " << distance * id << " to " << distance * (id + 1) << endl;
			mtx_cout.unlock();
			
			// MAIN FUNCTIONAL CORE
			bool PP = testForPrime(distance * id, distance * (id + 1));
			
			mtx_isPP.lock();
			isPotentialPrime[id] = PP;
			mtx_isPP.unlock();
			
			// mtx_cout.lock();
			// cout << "ID: " << id << " calculated " << numTesting << (isPotentialPrime[id]? " is not a prime":" could be a prime") << endl;
			// mtx_cout.unlock();
			
			ull temp = numTesting;
			
			// mtx_cout.lock();
			// cout << "ID: " << id << " waiting for other threads to finish " << endl;
			// mtx_cout.unlock();
			
			mtx_finished.lock();
			threadFinished[id] = true;
			mtx_finished.unlock();
			
			
			try{
				std::unique_lock<mutex> lk(mtx_finished);
				continueWork.notify_all();
				continueWork.wait(lk, otherThreadsFinished);
				continueWork.notify_all();
			} catch(...) {
				mtx_cout.lock();
				cout << "ID: " << id << " states that somthing went wrong... exiting " << endl;
				mtx_cout.unlock();
				exit(-1);
			}
			// mtx_cout.lock();
			// cout << "ID: " << id << " states that all other threads are finished " << endl;
			// mtx_cout.unlock();
			
			mtx_testIfFound.lock();
			if(numTesting == temp) {
				// mtx_cout.lock();
				// cout << "ID: " << id << " finishing up number test" << endl;
				// mtx_cout.unlock();
				
				int f = 0;
				for(int i = 0; i < NUM_OF_THREADS; ++i)
					f += isPotentialPrime[i];
				if(f == 4) {
					// mtx_cout.lock();
					// cout << "ID: " << id << " calculated " << numTesting << " is a Prime... ending" << endl;
					// mtx_cout.unlock();
					isPrime = true;	
				} else {
					// mtx_cout.lock();
					// cout << "ID: " << id << " calculated " << numTesting << " is not a Prime... ending" << endl;
					// mtx_cout.unlock();
					isPrime = false;
				}
			}
			mtx_testIfFound.unlock();
		}
		
		ReturnData run(ull tester) {
			using namespace std::chrono;
			using namespace std;
			
			vector<thread> threads; // vector storing threads
			
			numTesting = tester;
			
			auto startTime = steady_clock::now();
			for(int i = 0; i < NUM_OF_THREADS; ++i)
				threads.push_back(thread(threadWork, i));

			for(auto& t: threads) t.join();
			
			auto endTime = steady_clock::now();
			auto diff = endTime - startTime;
			auto nseconds = double(diff.count()) * steady_clock::period::num / steady_clock::period::den;
			
			ReturnData data;
			data.isPrime = isPrime;
			data.time = nseconds;
			
			return data;
		}
	}
}