// parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <list>
#include <thread>
#include <condition_variable>
#include <deque>
#include <algorithm>

std::string uiOutput(std::string inputFileName, std::string outputFileName, int totalLineCount, int lineCount, int percentage);
size_t stringCount(const std::string& referenceString, const std::string& subString);

#pragma region RegEx_Strings
enum NamesRegex { eTitle, eType, eSuspended, eEpisode, eYear, eCountry, eActors, eReleaseDate, eGenre, eRating, eUncredited, eVoice, eArchiveFootage, eAsPerson };

std::string Title = "\\\"?((.+)\\\"|.+)\\s(\\((\\d{4}\\/\\w+|[?]{4}\\/\\w+|[?]{4}|\\d{4}))\\)";
std::string Type = "\\((TV|V|VG)\\)";
std::string Suspended = "\\{\\{(SUSPENDED)\\}\\}";
std::string Episode = "(\\{(((.+)\\s\\(\\#(\\d+)\\.(\\d+)\\))|(\\((\\d+\\-\\d+\\-\\d+)\\))|(\\(\\#(\\d+)\\.(\\d+)\\))|(.+)(\\((V|TV|VG)\\)))\\})";
std::string Year = "\\t(\\d{4}|[?]{4})(-(\\d{4}|[?]{4}))?";
std::string Country = "\\t(\\S+(\\s\\S+(\\s\\S+(\\s\\S+(\\s\\S+)?)?)?)?)$";
std::string Actors = "(.+)\\t((\\\"(.+)\\\"|.+))\\s\\((\\d{4}\\/\\w+|[?]{4}\\/\\w+|[?]{4}|\\d{4})\\)\\s+(\\((V|TV|VG)\\))?(\\s+)?(\\((uncredited)\\))?(\\s+)?(\\{(((.+)\\s\\(\\#(\\d+)\\.(\\d+)\\))|(\\((\\d+\\-\\d+\\-\\d+)\\))|(\\(\\#(\\d+)\\.(\\d+)\\))|(.+))(\\((V|TV|VG)\\))?\\})?(\\s+)?(\\((.+)\\))?(\\s+)?(\\[(.+)\\])?((\\s+)(\\<\\d+\\>))?$";
std::string ReleaseDate = "\\t(\\w.+):\\s*(\\w+\\s\\d{4}|\\d{1,2}\\s\\w+\\s\\d{4}|\\d{4})(\\s\\((\\w+)\\))?(\\s\\((\\w+)\\))?(\\s\\((\\w+)\\))?";
std::string Genre = "\\t(\\w+[-]?\\w+)";
std::string Rating = "(\\S)(\\S)(\\S)(\\S)(\\S)(\\S)(\\S)(\\S)(\\S)(\\S)\\s+(\\d+)\\s+(\\d{1,2}\\.\\d)\\s+(.+)(\\((\\d{4}\\/\\w+|[?]{4}\\/\\w+|[?]{4}|\\d{4}))\\)";
std::string Uncredited = "\\((uncredited)\\)";
std::string Voice = "\\((voice)\\)";
std::string Archive = "(also\\s)?(archive\\s?footage)";
std::string AsPerson = "\\((as\\s.+)\\)";

NamesRegex CheckRegex(std::string const& inString) {
	if (inString == Title) return eTitle;
	if (inString == Type) return eType;
	if (inString == Suspended) return eSuspended;
	if (inString == Episode) return eEpisode;
	if (inString == Year) return eYear;
	if (inString == Country) return eCountry;
	if (inString == Actors) return eActors;
	if (inString == ReleaseDate) return eReleaseDate;
	if (inString == Genre) return eGenre;
	if (inString == Rating) return eRating;
	if (inString == Uncredited) return eUncredited;
	if (inString == Voice) return eVoice;
	if (inString == Archive) return eArchiveFootage;
	if (inString == AsPerson) return eAsPerson;
}

#pragma endregion RegEx_Strings

#pragma region Distro_Ratings
enum Distribution { eDot, e0, e1, e2, e3, e4, e5, e6, e7, e8, e9,eStar};

Distribution CheckDistro(std::string const& inString) {
	if (inString == ".") return eDot;
	if (inString == "0") return e0;
	if (inString == "1") return e1;
	if (inString == "2") return e2;
	if (inString == "3") return e3;
	if (inString == "4") return e4;
	if (inString == "5") return e5;
	if (inString == "6") return e6;
	if (inString == "7") return e7;
	if (inString == "8") return e8;
	if (inString == "9") return e9;
	if (inString == "*") return eStar;
}
#pragma endregion Distro_Ratings

int main() {
	std::string inputFileName;
	std::string outputFileName;

	std::list<std::string> regexList;

	//enum with output choices
	enum outputFilename { actresses, actors1, actors2, movies, countries, genres, releasedates, ratings};
//							0			1		2		3			4		5			6			7
	switch (2)
	{
	case actresses:
		inputFileName = "actresses.list"; //input
		outputFileName = "actresses.csv"; //output
		regexList = { Actors,AsPerson,Uncredited,Voice,Archive };
		break;
	case actors1:
		inputFileName = "actors1.list"; 
		outputFileName = "actors1.csv"; 
		regexList = { Actors,AsPerson,Uncredited,Voice,Archive };
		break;
	case actors2:
		inputFileName = "actors2.list";
		outputFileName = "actors2.csv";
		regexList = { Actors,AsPerson,Uncredited,Voice,Archive };
		break;
	case movies:
		inputFileName = "movies.list"; 
		outputFileName = "movies.csv"; 
		regexList = { Title,Type,Suspended,Episode,Year }; 
		break;
	case countries:
		inputFileName = "countries.list"; 
		outputFileName = "countries.csv"; 
		regexList = { Title,Type,Suspended,Episode,Country };
		break;
	case genres:
		inputFileName = "genres.list"; 
		outputFileName = "genres.csv"; 
		regexList = { Title,Type,Suspended,Episode,Genre }; 
		break;
	case releasedates :
		inputFileName = "release-dates.list"; 
		outputFileName = "release-dates.csv"; 
		regexList = { Title,Type,Suspended,Episode,ReleaseDate };
		break;
	case ratings:
		inputFileName = "ratings.list"; 
		outputFileName = "ratings.csv"; 
		regexList = { Rating,Type,Episode };
		break;
	default:
		break;
	}

	std::ifstream file(inputFileName);
	std::ifstream totalLineCountFile(inputFileName);
	std::ofstream outputFile(outputFileName);

	std::string line;
	std::smatch match;
	std::string seperator = ";";
	std::string name = "Lago, Fábio";

	//main
	int lineCount = 0;
	int totalLineCount = 0;
	int lastPercentage = 0;
	for (totalLineCount = 0; std::getline(totalLineCountFile, line); ++totalLineCount); //counts total amount of lines

	if (file.is_open()) {
		while (getline(file, line)) { //get nextline from file and store it in line
			lineCount++;
			
			//UI
			int percentage = int((double)lineCount / (double)totalLineCount * 100);
			if (percentage != lastPercentage) {
				lastPercentage = percentage;
				system("cls");
				std::cout << uiOutput(inputFileName, outputFileName, totalLineCount, lineCount, percentage);
			}

			//REGEX
			std::string output = "";
			for (auto regX : regexList) { //foreach expression in expression list => apply expression on line
				if (regex_search(line, match, std::regex(regX)))

					switch (CheckRegex(regX))
					{
					case eRating:
						//Distribution of the votes
						for (int i = 1; i < 11; i++)
						{
							switch (CheckDistro(match.str(i)))
							{
							case eDot:
								output += "0%" + seperator;
								break;
							case e0:
								output += "1-9%" + seperator;
								break;
							case e1:
								output += "10-19%" + seperator;
								break;
							case e2:
								output += "20-29%" + seperator;
								break;
							case e3:
								output += "30-39%" + seperator;
								break;
							case e4:
								output += "40-49%" + seperator;
								break;
							case e5:
								output += "50-59%" + seperator;
								break;
							case e6:
								output += "60-69%" + seperator;
								break;
							case e7:
								output += "70-79%" + seperator;
								break;
							case e8:
								output += "80-89%" + seperator;
								break;
							case e9:
								output += "80-89%" + seperator;
								break;
							case eStar:
								output += "100%" + seperator;
								break;
							default:
								output += "Error" + seperator;
								break;
							}
						}
						output += match.str(11) + seperator; //Votes
						output += match.str(12) + seperator; //Rating
						output += match.str(13) + seperator; //Title
						output += match.str(15) + seperator; //Year
						break;
					case eTitle:
						//Title
						if (match.str(2) != "") { output += match.str(2) + seperator; }
						else { output += match.str(1) + seperator; }

						//Year (depending on format [1999-2002 | 2001 | ???? ] )
						if (match.str(6) != "") {
							output += match.str(6) + seperator;
						}
						else if (match.str(5) != "") {
							output += match.str(5) + seperator;
						}
						else if (match.str(4) != "") {
							output += match.str(4) + seperator;
						}
						else { output += "Unknown" + seperator; } //if the year is "????"
						break;
					case eType:
						if (match.str(1) == "VG") { output += "Video Game" + seperator; }
						else if (match.str(1) == "V") { output += "Video" + seperator; }
						else if (match.str(1) == "TV") { output += "TV Show" + seperator; }
						break;
					case eSuspended:
						if (match.str(1) != "") output += "1" + seperator;
						break;
					case eEpisode:
						if (match.str(2) != "") {
							//Episode Name
							if ((match.str(4) != "" | match.str(12) != "")& match.str(12) != "{SUSPENDED}")
								output += match.str(4) + match.str(12) + seperator;
							else
								output += "NULL" + seperator;

							//Season Number
							if (match.str(5) != "" | match.str(10) != "")
								output += match.str(5) + match.str(10) + seperator;
							else
								output += "NULL" + seperator;

							//Episode Number
							if (match.str(6) != "" | match.str(11) != "" | match.str(8) != "")
								output += match.str(6) + match.str(11) + match.str(8) + seperator;
							else
								output += "NULL" + seperator;
						}
						break;
					case eYear:
						if (match.str(2) != "")
							output += match.str(1) + seperator;
						else
							output += match.str(1) + match.str(2) + seperator;
						break;
					case eCountry:
						output += match.str(1) + seperator;
						break;
					case eActors: //Actor Name
						if (match.str(1) != "\t\t") {
							name = match.str(1);
							output += name + seperator;
						}
						else { //No name print previous one (more than 1 movie)
							output += name + seperator;
						}
						// Movie name
						if (match.str(4) != "") {
							output += match.str(4) + seperator;
						}
						else { output += match.str(3) + seperator; }
						//Year
						if (match.str(5) != "") { output += match.str(5) + seperator; }
						else { output += "NULL" + seperator; }
						//Type (TV/V/VG)
						if (match.str(7) != "") {
							if (match.str(7) == "VG") { output += "Video Game" + seperator; }
							else if (match.str(7) == "V") { output += "Video" + seperator; }
							else if (match.str(7) == "TV") { output += "TV Show" + seperator; }
						}
						else { output += "NULL" + seperator; }
						//Episode
						if (match.str(12) != "" & match.str(12) != "{SUSPENDED}") {
							if (match.str(21) != "") { // (#1.1)
								output += "NULL" + seperator;
								output += match.str(21) + seperator;
								output += match.str(22) + seperator;
							}
							else if (match.str(15) != "") { // text (#1.1)
								output += match.str(15) + seperator;
								output += match.str(16) + seperator;
								output += match.str(17) + seperator;
							}
							else if (match.str(19) != "") { //date
								output += "NULL" + seperator;
								output += "NULL" + seperator;
								output += match.str(19) + seperator;
							}
							else if (match.str(23) != "") { // text only
								output += match.str(23) + seperator;
								output += "NULL" + seperator;
								output += "NULL" + seperator;
							}
						}
						else {
							output += "NULL" + seperator;
							output += "NULL" + seperator;
							output += "NULL" + seperator;
						}
						// [Role actor / actress plays]
						if (match.str(31) != "") {
							output += match.str(31) + seperator;
						}
						else { output += "NULL" + seperator; }

					break;					case eGenre:
						if (match.str() != "") output += match.str(1) + seperator;
						break;
					case eReleaseDate:
						output += match.str(1) + seperator;//Country
						output += match.str(2) + seperator;//Date
						if(match.str(4) !="") output += match.str(4) + match.str(6)+ match.str(8)+ seperator;//Location(s)
						else output += "NULL" + seperator;
						break;
					case eUncredited:
						if (match.str(1) != "")  output += "1" + seperator; 
						else { output += "NULL" + seperator; }
						break;
					case eVoice:
						if (match.str(1) != "") output += "1" + seperator;
						else { output += "NULL" + seperator; }
						break;
					case eArchiveFootage:
						if (match.str(2) != "") output += "1" + seperator;
						else output += "NULL" + seperator;
						break;
					case eAsPerson:
						if (match.str(1) != "") output += match.str(1) + seperator;
						else output += "NULL" + seperator;
						break;
					default:
						output += "Error Regex" + seperator;
						break;
					}
				else 
					if (CheckRegex(regX) == eEpisode)
						for (int i = 0; i < 3;i++)
							output += "NULL" + seperator;
					else 
						output += "NULL" + seperator;
			}
			//STRING COUNT
			outputFile << output << "\n";
			//std::cout << output << "\n";
		}
	}

	file.close();
	return 0;
}

//Method to count the amount of times a word occurs in a substring
size_t stringCount(const std::string& referenceString, const std::string& subString) {
	const size_t step = subString.size();

	size_t count(0);
	size_t pos(0);

	while ((pos = referenceString.find(subString, pos)) != std::string::npos) {
		pos += step;
		++count;
	}
	return count;
}

//generates the output for the UI.
//Displays the current file, outputfile, total amount of lines, Current line, percentage
std::string uiOutput(std::string inputFileName, std::string outputFileName, int totalLineCount, int lineCount, int percentage) {
	std::string output = "File name:		" + inputFileName + "\nOutput file:		" + outputFileName + "\n\nAmount of lines:	" + std::to_string(totalLineCount) + "\nCurrent line:		" + std::to_string(lineCount) + "\n\n";
	output += std::to_string(percentage) + "%\n";
	output += "[";
	for (int i = 0; i < 50; i++) {
		if (i < percentage / 2) output += '=';
		else if (i == percentage / 2) output += '>';
		else output += ' ';
	}
	output += ']';
	return output;
}