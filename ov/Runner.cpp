/* 

 */
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }

int main() {
	for(int i = 1; i < 11; ++i) {
		std::string c = "PrimeFinderT1.exe " + to_string(i) + " 1000000 0 >> a.txt";
		//cout << c;
		system(c.c_str());
	}
	for(int i = 1; i < 11; ++i) {
		std::string c = "PrimeFinderT2.exe " + to_string(i) + " 1000000  0 >> a.txt";
		//cout << c;
		system(c.c_str());
	}
	
	system("echo. >> a.txt");
}