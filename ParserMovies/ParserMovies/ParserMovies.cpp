// ParserMovies.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <regex>
#include <string>

const std::string Semicolon = ";";

void PrintMatches2(std::string str,
	std::regex reg) {

	// This holds the first match
	std::sregex_iterator currentMatch(str.begin(),
		str.end(), reg);

	// Used to determine if there are any more matches
	std::sregex_iterator lastMatch;

	// While the current match doesn't equal the last
	while (currentMatch != lastMatch) {
		std::smatch match = *currentMatch;
		std::cout << match.str() << "\n";
		currentMatch++;
	}
	std::cout << std::endl;
}


int main(){
	std::string str = "\"!Next?\" \(1994\)						Italy\n";
	std::regex line("\\t((\\w+[-.]?)\\s?){1,5}\\n");

	PrintMatches2(str, line);
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
