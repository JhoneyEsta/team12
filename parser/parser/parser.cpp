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

using namespace std;

string uiOutput(string inputFileName, string outputFileName, int totalLineCount, int lineCount, int percentage);
size_t stringCount(const std::string& referenceString, const std::string& subString);

#pragma region RegEx_Strings
enum NamesRegex { eTitle, eType, eSuspended, eEpisode, eYear, eCountry, eActors, eReleaseDate, eGenre, eRating};

std::string Title = "\\\"?((.+)\\\"|.+)\\s(\\((\\d{4})|\\([?]{4}|\\((\\d{4}\\/[IVX]{1,7}))\\)";
std::string Type = "\\((TV|V|VG)\\)";
std::string Suspended = "\\{\\{SUSPENDED\\}\\}";
std::string Episode = "\\{(.+)?\\(\\#(\\d{1,4}).(\\d{1,5})\\)\\}";
std::string Year = "\\t(\\d{4}|[?]{4})(-(\\d{4}|[?]{4}))?";
std::string Country = "\\t((\\w+[-.]?)\\s?){1,5}\\n";
std::string Actors = "(.+)  \\s?(\\\"?((.+)\\\"|.+)\\s(\\((\\d{4})|\\([?]{4}))(\\S+)?\\)\\s?\\s?(\\{.+\\})?(\\(.+\\))?(\\s{0,4})?(\\[.+\\])?\\s?";
std::string ReleaseDate = "\\t(\\w+):(\\w+\\s\\d{4}|\\d{1,2}\\s\\w+\\s\\d{4}|\\d{4})(\\t(\\(.+\\)))?";
std::string Genre = "\\t(\\s+)";
std::string Rating = "(\\s+)?(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(.+)\\((\\d+)\\)";

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
}

#pragma endregion RegEx_Strings

int main() {
	string inputFileName;
	string outputFileName;

	ifstream file(inputFileName);
	ifstream totalLineCountFile(inputFileName);
	ofstream outputFile(outputFileName);

	string line;
	smatch match;
	string seperator = "; ";

	list<string> expressionList;

	//enum with output choices
	enum outputFilename { actresses, actors, movies, countries, genres, releasedates, ratings};

	//ratings
	switch (actresses)
	{
	case actresses:
		inputFileName = "actresses.list"; //input
		outputFileName = "actresses.csv"; //output
		expressionList = { Actors };
		break;
	case actors:
		inputFileName = "actors.list"; 
		outputFileName = "actors.csv"; 
		expressionList = { Actors };
		break;
	case movies:
		inputFileName = "movies.list"; 
		outputFileName = "movies.csv"; 
		expressionList = { Title,Type,Suspended,Episode,Year }; 
		break;
	case countries:
		inputFileName = "countries.list"; 
		outputFileName = "countries.csv"; 
		expressionList = { Title,Type,Episode,Country };
		break;
	case genres:
		inputFileName = "genres.list"; 
		outputFileName = "genres.csv"; 
		expressionList = { Title,Type,Suspended,Episode,Genre }; 
		break;
	case releasedates :
		inputFileName = "release-dates.list"; 
		outputFileName = "release-dates.csv"; 
		expressionList = { Title,Type,Suspended,Episode,ReleaseDate };
		break;
	case ratings:
		inputFileName = "ratings.list"; 
		outputFileName = "ratings.csv"; 
		expressionList = { Rating };
		break;
	default:
		break;
	}

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
				cout << uiOutput(inputFileName, outputFileName, totalLineCount, lineCount, percentage);
			}

			//REGEX
			string output = "";
			std::string name;
			for (auto exp : expressionList) { //foreach expression in expression list => apply expression on line
				if (regex_search(line, match, regex(exp)))

					switch (CheckRegex(exp))
					{
					case eRating:
						if (match.str() != "") {
							if (match.str(2) != "") { output += match.str(2) + seperator; }
							if (match.str(3) != "") { output += match.str(3) + seperator; }
							if (match.str(4) != "") { output += match.str(4) + seperator; }
							if (match.str(5) != "") { output += match.str(5) + seperator; }
							if (match.str(6) != "") { output += match.str(6) + seperator; }
						}
						break;
					case eTitle:
						if (match.str() != "") {
							if (match.str(2) != "") { output += match.str(2) + seperator;}
							else { output += match.str(1) + seperator; }

							if (match.str(6) != "") {
								if (match.str(5) != "") {
									output += match.str(4) + seperator;
								}
								else { output += match.str(5) + seperator; }
							}
							else { output += "Unknown" + seperator; }
						}
						break;
					case eType:
						if (match.str() != "") {
							if (match.str(1) == "VG") { output += "Video Game" + seperator; }
							else if (match.str(1) == "V") { output += "Video" + seperator; }
							else if (match.str(1) == "TV") { output += "TV Show" + seperator; }
						}
						break;
					case eSuspended:
						if (match.str() != "") { output += match.str() + seperator; }
						break;
					case eEpisode:
						if (match.str(1) != "") {
							if (match.str(1) != "") { output += match.str(1) + seperator; }
							output += match.str(2) + seperator;
							output += match.str(3) + seperator;
						}if (match.str() != "") { output += match.str(1) + seperator; }		
						break;
					case eYear:
						if (match.str(2) != "") {
							output += match.str(1) + seperator;
						}
						else {
							output += match.str(1)+ match.str(2) + seperator;
						}
						break;
					case eCountry:
						output += match.str(1) + seperator;
						break;
					case eActors:
						if (match.str(1) != "") {
							name = match.str(1);
							output += name + seperator;
						}
						else {
							std::cout << name << "\n";
						}

						if (match.str(4) != "") {
							output += match.str(4) + seperator;
						}
						else {
							output += match.str(3) + seperator;
						}

						output += match.str(6) + seperator;

						if (match.str(8) != "") {
							output += match.str(8) + seperator;
						}

						if (match.str(9) != "") {
							output += match.str(9) + seperator;
						}

						if (match.str(11) != "") {
							output += match.str(11) + seperator;
						}
						break;
					case eGenre:
						if (match.str() != "") {
							output += match.str(1) + seperator;
						}
						break;
					case eReleaseDate:
						if (match.str() != "") {
							output += match.str(1) + seperator;
							output += match.str(2) + seperator;
							output += match.str(4) + seperator;
						}
						break;
					default:
						output += "Error" + seperator;
						break;
					}
				else
					output += "NULL" + seperator;
			}
			//STRING COUNT
			if (stringCount(output, "NULL") != expressionList.size()) { //if output only contains null => skip
				outputFile << output << endl;
			}
			std::cout << output << endl;
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
string uiOutput(string inputFileName, string outputFileName, int totalLineCount, int lineCount, int percentage) {
	string output = "File name:		" + inputFileName + "\nOutput file:		" + outputFileName + "\n\nAmount of lines:	" + to_string(totalLineCount) + "\nCurrent line:		" + to_string(lineCount) + "\n\n";
	output += to_string(percentage) + "%\n";
	output += "[";
	for (int i = 0; i < 50; i++) {
		if (i < percentage / 2) output += '=';
		else if (i == percentage / 2) output += '>';
		else output += ' ';
	}
	output += ']';
	return output;
}