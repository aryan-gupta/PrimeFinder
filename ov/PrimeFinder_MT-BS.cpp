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

int NUM_OF_THREADS = 2;
bool dP;
long START = 1;
long END   = 1000;
				// 1,000,000

std::mutex mtx;
std::vector<std::vector<unsigned int> > foundPrimes;

bool testForPrime(const unsigned int num) {
	for(unsigned int i = 2; i < num; ++i) {
		if(num % i == 0) {
			return false;
		}
	}
	return true;
}

void lookForPrimes(const unsigned int start, const unsigned int end) {
	std::vector<unsigned int>foundPrimes_local;
	
	for(unsigned int i = start; i <= end; i++) {
		if(testForPrime(i)) {
			foundPrimes_local.push_back(i);
		}
	}
	mtx.lock();
	foundPrimes.push_back(foundPrimes_local);
	mtx.unlock();
}

void displayPrimes() {
	std::cout << std::endl;
	for(auto& v: foundPrimes) {
		for(auto i: v)
			std::cout << i << "\t";
		std::cout << std::endl;
	}
}


int main(int argc, char* argv[]) {
	using namespace std;
	using namespace std::chrono;
	
	NUM_OF_THREADS = atoi(argv[1]);
	END = static_cast<long>(atoi(argv[2]));
	dP = static_cast<bool>(atoi(argv[3]));
	
	vector<thread> threads; // vector storing threads
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
		threads.push_back(thread(lookForPrimes, START + (distance * (i)), START + (distance * (i + 1))));
	threads.push_back(thread(lookForPrimes, START + (distance * (NUM_OF_THREADS - 1)), END));

	for(auto& t: threads) t.join();
	
	auto endTime = steady_clock::now();
	auto diff = endTime - startTime;
	auto nseconds = double(diff.count()) * steady_clock::period::num / steady_clock::period::den;
	
	if(dP) {
		cout << endl << endl;
		displayPrimes();
		cout << endl << endl;
	}
	
	int size = 0;
	for(auto& v: foundPrimes)
		for(auto& i: v)
			size++;
	
	cout << size << " primes found between " << START << " and " << END << " using " << NUM_OF_THREADS << " cores in " << nseconds << " seconds." << endl;
	
	return 0;
}