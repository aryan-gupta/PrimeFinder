/* 
 * Copyright (c) The Gupta Empire - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Written by Aryan Gupta <me@theguptaempire.net>
 * 
 * ==========================================
 * @author Aryan Gupta
 * @project Prime Finder
 * @title Prime Finder Using Multiple Ways
 * @date 2017-02-12 (YYYY-MM-DD)
 * @version 5.3
 * @description Finds prime numbers by multithreading using sections,
 * 				multithreading using single numbers, by single threading,
 * 				also tests if a number is prime, and finds the next 
 *				prime number
 * ==========================================
 */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <chrono>
#include <stdlib.h>

#include "global.h"

namespace MThreaded {
	namespace SpecificNumber {
		
		bool testForPrime(const unsigned long long start, const unsigned long long end);
		
		bool otherThreadsFinished(/*int id*/);
		
		void threadWork(int id);
		
		ReturnData run(unsigned long long tester);
	}
}