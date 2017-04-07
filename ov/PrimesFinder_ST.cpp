/* 

 */
 
#include <cstdlib>
#include <iostream>
#include <vector>

#include <chrono>

//#include <thread>
//#include <mutex>

//mutex mtx;
std::vector<std::vector<unsigned int> > foundPrimes;
std::vector<unsigned int> numsLooked;

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
			//mtx.lock();
			foundPrimes_local.push_back(i);
			//std::cout << i << std::endl;
			//mtx.unlock();
		}
		if(i % 10000 == 0)
			std::cout << ".";
	}
	foundPrimes.push_back(foundPrimes_local);
}

void displayPrimes() {
	for(auto& v: foundPrimes)
		for(auto i: v)
			std::cout << i << "\t";
}

int main() {
	using namespace std::chrono;
	
	auto start = steady_clock::now();
	lookForPrimes(3, 100000);
	auto end = steady_clock::now();
	
	
	auto diff = end - start;
	auto nseconds = double(diff.count()) * steady_clock::period::num / steady_clock::period::den;
	
	//displayPrimes();
	
	std::cout << "\nTIME TO EXEC:: \t" << nseconds / 60 << " minutes" << std::endl;
	return 0;
}