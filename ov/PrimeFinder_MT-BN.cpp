/* 

 */
 
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

#if (defined (LINUX) || defined (__linux__))
	#include <thread>
	#include <mutex>
#elif (defined (_WIN32) || defined (_WIN64))
	#include "mingw.thread.h"
	#include "mingw.mutex.h"
#endif

int NUM_OF_THREADS = 3;
bool dP = false;
long START = 1;
long END   = 10000;
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

void displayPrimes() {
	std::cout << std::endl;
	for(auto& v: foundPrimes) {
		std::cout << v << "\t";
	}
}

int main(int argc, char* argv[]) {
	using namespace std::chrono;
	using namespace std;
	
	if(argc == 4) {
		NUM_OF_THREADS = atoi(argv[1]);
		END = static_cast<long>(atoi(argv[2]));
		dP = static_cast<bool>(atoi(argv[3]));
	}
	
	vector<thread> threads; // vector storing threads
	
	auto startTime = steady_clock::now();
	for(int i = 0; i < NUM_OF_THREADS; ++i)
		threads.push_back(thread(lookForPrimes));

	for(auto& t: threads) t.join();
	
	auto endTime = steady_clock::now();
	auto diff = endTime - startTime;
	auto nseconds = double(diff.count()) * steady_clock::period::num / steady_clock::period::den;
	
	if(dP) {
		cout << endl << endl;
		displayPrimes();
		cout << endl << endl;
	}
	
	cout << foundPrimes.size() << " primes found between " << START << " and " << END << " using " << NUM_OF_THREADS << " cores in " << nseconds << " seconds." << endl;
	
	return 0;
}
