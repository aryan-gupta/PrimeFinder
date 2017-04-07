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

#if (defined (LINUX) || defined (__linux__))
	#include <thread>
	#include <mutex>
	#include <condition_variable>
#elif (defined (_WIN32) || defined (_WIN64))
	#include ".\h\mingw.thread.h"
	#include ".\h\mingw.mutex.h"
#endif

#include ".\h\global.h"

typedef unsigned long long ull;

namespace MThreaded {
	namespace NextPrime {
		using std::vector;
		using std::cout;
		using std::endl;
		using std::thread;
		using std::mutex;
		
		ull numTesting;
		vector<bool> threadFinished(NUM_OF_THREADS, false);
		vector<bool> isPotentialPrime(NUM_OF_THREADS, true);
		
		std::condition_variable continueWork;
		mutex test;
		
		mutex mtx_testIfFound;
		mutex mtx_finished;
		mutex mtx_cout;
		mutex mtx_isPP;
		
		bool testForPrime(const ull start, const ull end) {
			for(unsigned int i = start; i < end; ++i) {
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
			for(int i = 0; i < NUM_OF_THREADS; ++i) {
				f += threadFinished[i];
			}
			if(f == 4)
				return true;
			return false;
		}
		
		void resetThread(int id) {
			mtx_finished.lock();
			threadFinished[id] = false;
			mtx_finished.unlock();
			mtx_isPP.lock();
			isPotentialPrime[id] = false;
			mtx_isPP.unlock();
		}
		
		void threadPool(int id) {
			while(true) {
				resetThread(id);
				
				long distance = numTesting / NUM_OF_THREADS;
				
				mtx_cout.lock();
				cout << "ID: " << id << " testing " << numTesting << " from " << distance * id << " to " << distance * (id + 1) << endl;
				mtx_cout.unlock();
				
				// MAIN FUNCTIONAL CORE
				bool PP = testForPrime(distance * id, distance * (id + 1));
				
				mtx_isPP.lock();
				isPotentialPrime[id] = PP;
				mtx_isPP.unlock();
				
				mtx_cout.lock();
				cout << "ID: " << id << " calculated " << numTesting << (isPotentialPrime[id]? " is not a prime":" could be a prime") << endl;
				mtx_cout.unlock();
				
				long temp = numTesting;
				
				mtx_cout.lock();
				cout << "ID: " << id << " waiting for other threads to finish " << endl;
				mtx_cout.unlock();
				
				mtx_finished.lock();
				threadFinished[id] = true;
				mtx_finished.unlock();
				
				
				std::unique_lock<mutex> lk(test);
				continueWork.notify_all();
				continueWork.wait(lk, otherThreadsFinished);
				continueWork.notify_all();
				/*
				while(!otherThreadsFinished()) {
					mtx_cout.lock();
					cout << "ID: " << id << " starting sleep with " << temp << endl;
					mtx_cout.unlock();
					//std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
				/*
				std::unique_lock<mutex> lk(test);
				continueWork.notify_all();
				continueWork.wait_for(lk, std::chrono::milliseconds(50000), otherThreadsFinished);
				continueWork.notify_all();
				/*
				//continueWork.notify_all();
				while(!otherThreadsFinished()) {
					continueWork.wait(lk);
					//continueWork.notify_all();
				}
				//*/
				
				mtx_cout.lock();
				cout << "ID: " << id << " states that all other threads are finished " << endl;
				mtx_cout.unlock();
				
				mtx_testIfFound.lock();
				if(numTesting == temp) {
					mtx_cout.lock();
					cout << "ID: " << id << " finishing up number test" << endl;
					mtx_cout.unlock();
					
					int f = 0;
					for(int i = 0; i < NUM_OF_THREADS; ++i)
						f += isPotentialPrime[i];
					if(f == 4) {
						mtx_cout.lock();
						cout << "ID: " << id << " calculated " << numTesting << " is a Prime... ending" << endl;
						mtx_cout.unlock();
						mtx_testIfFound.unlock();	
						break;
					} else {
						mtx_cout.lock();
						cout << "ID: " << id << " calculated " << numTesting << " is not a Prime... continuing to " << numTesting + 1 << endl;
						mtx_cout.unlock();
					}
					numTesting++;
				}
				mtx_testIfFound.unlock();
			}
		}
		
		ReturnData run(ull tester) {
			using namespace std::chrono;
			using namespace std;
			
			vector<thread> threads; // vector storing threads
			
			threads.reserve(NUM_OF_THREADS + 5);
			
			numTesting = tester;
			
			auto startTime = steady_clock::now();
			for(int i = 0; i < NUM_OF_THREADS; ++i)
				threads.push_back(thread(threadPool, i));

			for(auto& t: threads) t.join();
			
			auto endTime = steady_clock::now();
			auto diff = endTime - startTime;
			auto nseconds = double(diff.count()) * steady_clock::period::num / steady_clock::period::den;
			
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
	}
}