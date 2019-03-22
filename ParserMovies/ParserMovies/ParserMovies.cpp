// ParserMovies.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <regex>
#include <string>

const std::string Semicolon = ";";

void ParseMovies(std::string str, std::regex line) {

		// This holds the first match
		std::sregex_iterator currentMatch(str.begin(),
			str.end(), line);

		// Used to determine if there are any more matches
		std::sregex_iterator lastMatch;

				//Title+ReleaseYearZ
		std::regex title("\\\"?((.+)\\\"|.+)\\s(\\((\\d{4})|\\([?]{4})");
		std::string pTitle;
		std::string pReleaseYear;

		// This holds the first match		

		// While the current match doesn't equal the last
		while (currentMatch != lastMatch) {
			std::smatch match = *currentMatch;
			match.str(1).replace(str.find(Semicolon), Semicolon.length(), ":");

				//Title+ReleaseYear
			std::sregex_iterator matchTitle(match.str(1).begin(),
				match.str(1).end(), title);

			while (matchTitle != lastMatch) {
				std::smatch Title = *matchTitle;

				if (Title.str() != "") {
					if (Title.str(2) == "") { pTitle = Title.str(1) + ";"; }
					else { pTitle = Title.str(2) + ";"; }

					if (Title.str(4) != "") { pReleaseYear = Title.str(4) + ";"; }
					else { pReleaseYear = "Unknown;"; }
				}
			}
			std::string csvCode = pTitle + pReleaseYear +"\n";

			std::cout << csvCode;
			currentMatch++;
		}
		std::cout << std::endl;

	while (std::regex_search(str, matches, reg)) {

		//ParseMovies(matches.str(1), title, SUSPENDED, episode, episodeYear, type);

		// eliminate the previous match and create
		// a new string to search
		str = matches.suffix().str();

		std::cout << "\n";
	}
	
	std::sregex_iterator matchSuspended(str.begin(),
		str.end(), suspended);

	std::sregex_iterator matchEpisode(str.begin(),
		str.end(), episode);
	
	std::sregex_iterator matchEpYear(str.begin(),
		str.end(), epYear);

	std::sregex_iterator matchType(str.begin(),
		str.end(), type);


	// While the current match doesn't equal the last
	while (matchTitle != lastMatch) {
		std::smatch Suspended = *matchSuspended;
		std::smatch Episode = *matchEpisode;
		std::smatch EpYear = *matchEpYear;
		std::smatch Type = *matchType;
		
		std::string pSeason;
		std::string pEpisode;
		std::string pEpName;
		std::string pEpYear;
		std::string pType;

		if (Episode.str(1) != "") {
			if (Episode.str(1) != "") {pEpName = Episode.str(1) + ";";}
			pSeason = Episode.str(2) + ";";
			pEpisode = Episode.str(3) + ";";
		}if (EpYear.str() != "") {pEpYear = EpYear.str(1) + ";";}
		if (Type.str() != "") {
			if (Type.str(1) == "VG") { pType = "Video Game;"; }
			else if (Type.str(1) == "V") { pType = "Video;"; }
			else if (Type.str(1) == "TV") { pType = "TV Show;"; }
		}

		
		matchTitle++;

		if (matchTitle == lastMatch) {
			std::string csvCode = pTitle + pReleaseYear + pEpName + pSeason + pEpisode + pEpYear;
			std::cout << csvCode << std::endl;
		}
	}
}


void PrintMatches(std::string str, std::regex reg) {
	std::smatch matches;

	std::regex SUSPENDED("\\{\\{SUSPENDED\\}\\}");
	std::regex episode("\\{(.+)?\\(\\#(\\d{1,4}).(\\d{1,5})\\)\\}");
	std::regex episodeYear("[^(]\\s(([\\d]{4})-?([\\d]{4})?)\\s?\\n");
	std::regex type("\\((TV|V|VG)\\)");

	while (std::regex_search(str, matches, reg)) {

		matches.str(1).replace(str.find(Semicolon), Semicolon.length(), ":");
		//ParseMovies(matches.str(1), title, SUSPENDED, episode, episodeYear, type);

		// eliminate the previous match and create
		// a new string to search
		str = matches.suffix().str();

		std::cout << "\n";
	}
}

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
	std::string str = "\"!Next?\" (1994) (TV)	{{SUSPENDED}}					1994-1995\n\"'t Zonnetje; in huis\" (????) {Val es effe lekker dood (9.8)}	2003\nCommandment Keeper Church, Beaufort South Carolina, May 1940 (1940)	1940 \n";
	std::regex line("(.+)\n");

	ParseMovies(str, line);
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
