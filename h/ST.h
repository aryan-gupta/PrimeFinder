#ifndef _ST_H
#define _ST_H
namespace SThreaded {
	bool testForPrime(const unsigned int num);

	void lookForPrimes(const unsigned int start, const unsigned int end);

	void displayPrimes();

	extern ReturnData run();
}
#endif


