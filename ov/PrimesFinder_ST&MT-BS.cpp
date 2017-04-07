/* 

 */
 
#include <cstdlib>
#include <iostream>
#include <vector>

#include <chrono>

#if (defined (LINUX) || defined (__linux__))
	#include <thread>
	#include <mutex>
#elif (defined (_WIN32) || defined (_WIN64))
	#include "mingw.thread.h"
	#include "mingw.mutex.h"
#endif

int NUM_OF_THREADS = 1;
const int MAX_NOT = 5000;
const long START = 3;
const long END   = 1000000;
				// 1,000,000

std::mutex mtx;
std::vector<std::vector<unsigned int> > foundPrimes_MT;
std::vector<std::vector<unsigned int> > foundPrimes_ST;

bool testForPrime(const unsigned int num) {
	for(unsigned int i = 2; i < num; ++i) {
		if(num % i == 0) {
			return false;
		}
	}
	return true;
}

void lookForPrimes_MT(const unsigned int start, const unsigned int end) {
	std::vector<unsigned int>foundPrimes_local;
	
	for(unsigned int i = start; i <= end; i++) {
		if(testForPrime(i)) {
			foundPrimes_local.push_back(i);
		}
		//if(i % 10000 == 0)
		//	std::cout << ".";
	}
	mtx.lock();
	foundPrimes_MT.push_back(foundPrimes_local);
	mtx.unlock();
}

void lookForPrimes_ST(const unsigned int start, const unsigned int end) {
	std::vector<unsigned int>foundPrimes_local;
	
	for(unsigned int i = start; i <= end; i++) {
		if(testForPrime(i)) {
			foundPrimes_local.push_back(i);
		}
		//if(i % 10000 == 0)
		//	std::cout << ".";
	}
	foundPrimes_ST.push_back(foundPrimes_local);
}

void displayPrimes_MT() {
	std::cout << std::endl;
	for(auto& v: foundPrimes_MT) {
		for(auto i: v)
			std::cout << i << "\t";
		std::cout << std::endl;
	}
}

void displayPrimes_ST() {
	std::cout << std::endl;
	for(auto& v: foundPrimes_ST) {
		for(auto i: v)
			std::cout << i << "\t";
		std::cout << std::endl;
	}
}

double runMT() {
	using namespace std::chrono;
	
	std::vector<std::thread> threads; // vector storing threads
	long distance = ((END - START) / NUM_OF_THREADS);
	
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
		threads.push_back(std::thread(lookForPrimes_MT, START + (distance * (i)), START + (distance * (i + 1))));
	threads.push_back(std::thread(lookForPrimes_MT, START + (distance * (NUM_OF_THREADS - 1)), END));

	for(auto& t: threads) t.join();
	
	auto endTime = steady_clock::now();
	auto diff = endTime - startTime;
	auto nseconds = double(diff.count()) * steady_clock::period::num / steady_clock::period::den;
	
	return nseconds;
}

double runST() {
	using namespace std::chrono;
	
	auto start = steady_clock::now();
	lookForPrimes_ST(START, END);
	
	auto end = steady_clock::now();
	auto diff = end - start;
	auto nseconds = double(diff.count()) * steady_clock::period::num / steady_clock::period::den;
	
	return nseconds;
}

int main() {
	using namespace std;
	
	cout << "Non-threaded: " << runST() << endl;
		
	for(;NUM_OF_THREADS <= MAX_NOT; NUM_OF_THREADS++)
		cout << NUM_OF_THREADS << ", " << runMT() << endl;

	displayPrimes_ST();
	return 0;
}