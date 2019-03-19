// ParserMovies.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <regex>
#include <string>
//
//void PrintMatches(std::string str, std::regex reg) {
//	std::smatch matches;
//
//	while (std::regex_search(str, matches, reg)) {
//
//		std::regex title("^\"[\\w!?$#*!?.& \'()+-/]+\"$");
//		std::regex releaseYear("([\\d]{4})");
//		//std::regex years("\"[\\w!?$#*!?.& \'()+-/]{1,50}\"");
//
//		std::regex episode("{[\\w!?$#*!?.& \'()+-/]{1,50}}");
//		//std::regex seasonEpisode("\\([\d]{1,2}.\d\\)");
//		std::regex seasonEpisode("\\([\d]{2,4})");
//		std::regex episodeYear("([\\d]{4})");
//		std::regex years("([\\d]{4})");
//
//		//$#*!?.& \'()+-/
//
//		ParseMovies(matches.str(1), title, year, episode, seasonEpisode, episodeYear, );
//
//		// eliminate the previous match and create
//		// a new string to search
//		str = matches.suffix().str();
//
//		std::cout << "\n";
//	}
//}

// ----- END BEGINNING REGEX -----

// ----- ITERATOR REGEX -----

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

// ----- END ITERATOR REGEX -----

void ParseMovies(std::string str, std::regex title, std::regex releaseYear, std::regex epName, std::regex seasonEpisode, std::regex epYear, std::regex years) {

	// This holds the first match
	std::sregex_iterator matchTitle(str.begin(),
		str.end(), title);

	std::sregex_iterator matchReleasereleaseYears(str.begin(),
		str.end(), releaseYear);

	std::sregex_iterator matchepName(str.begin(),
		str.end(), epName);

	std::sregex_iterator matchSeasonEpisode(str.begin(),
		str.end(), seasonEpisode);

	std::sregex_iterator matchEpYear(str.begin(),
		str.end(), epYear);

	std::sregex_iterator matchYears(str.begin(),
		str.end(), epYear);

	// Used to determine if there are any more matches
	std::sregex_iterator lastMatch;

	// While the current match doesn't equal the last
	while (matchTitle != lastMatch) {
		std::smatch Title = *matchTitle;
		std::smatch releaseYear = *matchReleasereleaseYears;
		std::smatch epName = *matchepName;
		std::smatch seasonEpisode = *matchSeasonEpisode;
		std::smatch epYear = *matchEpYear;
		std::smatch Years = *matchYears;

		std::string pTitle = Title.str() + ";";
		std::string pReleaseYear = releaseYear.str() + ";";
		std::string pSeasonEpisode = seasonEpisode.str() + ";";
		std::string pEpName = epName.str() + ";";
		std::string pEpYear = epYear.str() + ";";
		std::string pYears = Years.str() + ";";

		if (++matchTitle == lastMatch) {
			std::string csvCode = pTitle + pReleaseYear + pEpName + pSeasonEpisode + pEpYear + pYears;
			std::cout << csvCode << std::endl;
		}
	}
	std::cout << std::endl;
}

int main()
{
	std::string str = "\"!Next?\" (1994)						1994-1995\n\"'t Zonnetje in huis\" (1993) {Val es effe lekker dood (9.8)}	2003\n";
	std::regex line("(.+)\n");
	std::regex title("^\"([.+])\"$");

	PrintMatches2(str, title);
	PrintMatches2(str, line);
	//std::regex reg("\"[\\w!?]{1,50}\" ([\\d]{4})[\\s][\\d]{4}-?[\\d]{4}?");
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
