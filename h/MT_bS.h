#ifndef _MT_BS_H
#define _MT_BS_H
namespace MThreaded {
	namespace bySection {		
		bool testForPrime(const unsigned long long num);

		void lookForPrimes(const unsigned long long start, const unsigned long long end);

		void displayPrimes();

		ReturnData run();
	}
}
#endif

