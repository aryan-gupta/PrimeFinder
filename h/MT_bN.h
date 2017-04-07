#ifndef _MT_BN_H
#define _MT_BN_H
namespace MThreaded {
	namespace byNumber {
		bool testForPrime(const unsigned long num);

		void lookForPrimes();

		void displayPrimes();

		ReturnData run();
	}
}
#endif