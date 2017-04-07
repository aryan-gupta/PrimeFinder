/* 

 */
 
#include <cstdlib>
#include <iostream>
#include <vector>

#include <chrono>

#include "mingw.thread.h"
#include "mingw.mutex.h"
//#include "mingw.condition_variable.h"
//#include <mutex>

const int NUM_OF_THREADS = 64;
const long START = 1;
const long END   = 1000;
				// 10,000,000

std::mutex mtx;
std::vector<long> foundPrimes;
std::vector<long> testedPrimes(1, START);

bool testForPrime(const unsigned long num) {
	for(unsigned int i = 2; i < num; ++i) {
		if(num % i == 0) {
			return false;
		}
	}
	return true;
}

void lookForPrimes() {
	tRunning = true;
	while(true) {
		mtx.lock();
		long num = testedPrimes.back() + 1;
		testedPrimes.push_back(num);
		mtx.unlock();
		
		if(testForPrime(num)) {
			mtx.lock();
			foundPrimes.push_back(num);
			//std::cout << num << "\t";
			mtx.unlock();
		}
		
		if(num > END)
			break;
	}
}

void organizeData() {
	while(tRunning) {
		for(long i = 1; i < foundPrimes.size() - 1; ++i) {
			if(foundPrimes[i - 1] > foundPrimes[i]) {
				mts.lock();
				long temp = foundPrimes[i - 1];
				foundPrimes[i - 1] = foundPrimes[i];
				foundPrimes[i] = temp;
				mtx.unlock();
			} else if(foundPrimes[i + 1] < foundPrimes[i]) {
				mts.lock();
				long temp = foundPrimes[i + 1];
				foundPrimes[i + 1] = foundPrimes[i];
				foundPrimes[i] = temp;
				mtx.unlock();
			} else {}
		}
	}
}

void displayPrimes() {
	std::cout << std::endl;
	for(auto& v: foundPrimes) {
		std::cout << v << "\t";
	}
}


int main() {
	using namespace std::chrono;
	using namespace std;
	
	vector<thread> threads; // vector storing threads
	
	auto startTime = steady_clock::now();
	for(int i = 0; i < NUM_OF_THREADS; ++i)
		threads.push_back(thread(lookForPrimes));
	thread organizer = thread(organizeData));

	for(auto& t: threads) t.join();
	tRunning = false;
	organizer.join();
	
	auto endTime = steady_clock::now();
	auto diff = endTime - startTime;
	auto nseconds = double(diff.count()) * steady_clock::period::num / steady_clock::period::den;
	
	displayPrimes();
	cout << endl << endl << foundPrimes.size() << " primes found between " << START << " and " << END << " in " << nseconds << " seconds." << endl;

	return 0;
}