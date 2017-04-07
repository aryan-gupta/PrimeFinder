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
 * @title Prime Finder Using Multiple Ways
 * @date 2017-02-12 (YYYY-MM-DD)
 * @fversion 3.3.0
 * @description Finds prime numbers by multithreading using sections,
 * 				multithreading using single numbers, by single threading,
 * 				also tests if a number is prime, and finds the next 
 *				prime number
 * ====================================================================================
 */

#include "info.h"
#define _PRINTINFO std::cout << '\n' << R_PROJNAME << " v" << R_PVERSION << " by " << R_AUTHOR << '\n' << R_COPYRIGHT << '\n' << R_COMMENTS << "\n\n\n" // Legal and Informational

#include <string.h>
#include <stdio.h>
#include <iostream>

#include ".\h\global.h"
#include ".\h\MT_bN.h"
#include ".\h\MT_bS.h"
#include ".\h\MT_SN.h"
//#include ".\h\MT_NP.h"
#include ".\h\ST.h"

typedef unsigned long long ull;

int NUM_OF_THREADS = 4; // stores the number of threads the program is going to use
bool dP = false; // should we display the found primes
bool dAll = false; // shoudl we display primes after each meathod
const int START = 2; // where to start when looking for primes
ull END = 100000; // where to end looking for primes

struct parseInfo {
	bool testSpecificPrime = false;
	bool findNextPrime = false;
	ull testNum = 0;
};

void helpText() {
	using std::cout;
	using std::endl;
	cout << R_DESCRIPTION << endl << endl;
	cout << "HELP:\n" << "Argument\tDefault\t\tDetails" << endl;
	cout << "-t \t\t"	 << 4		 << "\tUse this argument to specify the number of threads\n"
		 << "-m \t\t"	 << 100000   << "\tUse this argument to specify the maximum number\n"
		 << "-r \t\t"	 << "None"   << "\tUse this argument to specify the maximum number\n"
		 << "-n \t\t"	 << "None"   << "\tUse this argument to specify the maximum number\n"
		 << "-d \t\t"    << "False"  << "\tUse this switch to specify whether to displayt the found primes\n"
		 << "-dall\t\t"  << "False"  << "\tUse this switch to specify whether to primes from all threads\n" << endl << endl;
		 
	cout << "Example: .\\PrimeFinder.exe -t 7 -m 500000 -d -dall" << endl;
	cout << "Example: .\\PrimeFinder.exe -t 4 -r 500000000" << endl;
	cout << "Example: .\\PrimeFinder.exe -t 7 -e 300000 -d" << endl;
	
	cout << endl;
	exit(1);
}

parseInfo parse(int argc, char* argv[]) {
	parseInfo info;
	for(int i = 1; i < argc; ++i) {
		if       (strcmp(argv[i],"-t") == 0) {
			NUM_OF_THREADS = atoi(argv[i + 1]);
			i++;
		} else if(strcmp(argv[i], "-m") == 0) {
			END = atoll(argv[i + 1]);
			i++;
		} else if(strcmp(argv[i], "-r") == 0) {
			info.testNum = atoll(argv[i + 1]);
			info.testSpecificPrime = true;
			i++;
		} else if(strcmp(argv[i], "-n") == 0) {
			info.testNum = atoll(argv[i + 1]);
			//info.findNextPrime = true;
			i++;
		} else if(strcmp(argv[i], "-d") == 0) {
			dP = true;
		} else if(strcmp(argv[i], "-dall") == 0) {
			dAll = true;
		} else if(strcmp(argv[i], "/?") == 0 || strcmp(argv[i], "-?") == 0 )
			helpText();
		else
			helpText();
	}
	return info;
}

int main(int argc, char* argv[]) {
	_PRINTINFO;
	
	using std::cout;
	using std::endl;
	
	parseInfo info = parse(argc, argv);
	
	if(info.testSpecificPrime) {
		cout << "Testing " << info.testNum << endl;
		ReturnData SN = MThreaded::SpecificNumber::run(info.testNum);
		cout << info.testNum << " is" << ((SN.isPrime)? " ":" not ") <<"a prime. " << "It took " << NUM_OF_THREADS << " cores " << SN.time << " seconds to caculate." << endl << endl;	
	}/* else if (info.findNextPrime) { 
		cout << "Finding next prime after " << info.testNum << endl;
		ReturnData NP = NextPrime::run(info.testNum);
		cout << NP.nextPrime << " is the next prime after " << info.testNum << "It took " << NUM_OF_THREADS << " cores " << NP.time << " seconds to caculate." << endl << endl;
	} */else {
		// Single Thread
		cout << "Running Single Thread: \n";
		ReturnData ST = SThreaded::run();
		cout << ST.primesFound << " primes found between " << START << " and " << END << " using " << 1 << " cores in " << ST.time << " seconds." << endl << endl;	
		// Multi Thread using sections
		cout << "Running Multi Thread by Sections: \n";
		ReturnData MTBS = MThreaded::bySection::run();
		cout << MTBS.primesFound << " primes found between " << START << " and " << END << " using " << NUM_OF_THREADS << " cores in " << MTBS.time << " seconds." << endl << endl;
		// Multi Thread using individual numbers or a thread pool
		cout << "Running Multi Thread by Individual Numbers (thread pool): \n";
		ReturnData MTBN = MThreaded::byNumber::run();
		cout << MTBN.primesFound << " primes found between " << START << " and " << END << " using " << NUM_OF_THREADS << " cores in " << MTBN.time << " seconds." << endl << endl;
	}
	
	if(!dAll && dP) {
		MThreaded::byNumber::displayPrimes();
	}
	
	std::cout << "Press enter to quit...";
	std::cin.get();
	
	return 0;
}