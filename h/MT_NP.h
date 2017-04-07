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
 * @title Prime Finder Next Prime Header
 * @date 2017-02-17 (YYYY-MM-DD)
 * @fversion 1.1.0
 * @description Finds next prime number after a number
 * ====================================================================================
 */

#include ".\h\global.h"

namespace MThreaded {
	namespace NextPrime {
		
		bool testForPrime(const unsigned long long start, const unsigned long long end);
		
		bool otherThreadsFinished(/*int id*/);
		
		void resetThread(int id)
		
		void threadPool(int id);
		
		ReturnData run(unsigned long long tester);
	}
}