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
	enum outputFilename { actressesRoles, actresses, actorsRoles, actors, directed, directors, movies, countries, genres, soundtracks, ratings };

	
	//ratings
	switch (actressesRoles)
	{
	case actressesRoles:
		inputFileName = "actresses.list"; //input actresses
		outputFileName = "roles.csv"; //output actresses
		expressionList = { "" }; //actresses roles koppel
		//1= ^.*?(?=\t) //actress //748 steps
		//2= (\t(.*) \([0-9]){1}//movie //3066 steps
		//3= \(([0-9]...)\){1} //year //241 steps
		//4= \[(.*)\]{1} //role //121 steps
		break;
	case actresses:
		inputFileName = "actresses.list"; //input actresses
		outputFileName = "actors.csv"; //output actresses
		expressionList = { "", "","","" }; //actresses
		//1=
		//2=
		//3=
		//4=
		break;
	case actorsRoles:
		inputFileName = "actors.list"; //input actors
		outputFileName = "roles.csv"; //output actors
		expressionList = { "" }; //actors roles koppel
		//1=
		//2=
		//3=
		//4=
		break;
	case actors:
		inputFileName = "actors.list"; //input actors
		outputFileName = "actors.csv"; //output actors
		expressionList = { "", "","","" }; //actors
		//1=
		//2=
		//3=
		//4=
		break;
	case directed:
		inputFileName = "directors.list"; //input directed
		outputFileName = "directed.csv"; //output directed
		expressionList = { "", "","","" }; //directed koppel
		//1=
		//2=
		//3=
		//4=
		break;
	case directors:
		inputFileName = "directors.list"; //input directors
		outputFileName = "directors.csv"; //output directors
		expressionList = { "", "","","" }; //directors
		//1=
		//2=
		//3=
		//4=
		break;
	case movies:
		inputFileName = "movies.list"; //input movies
		outputFileName = "movies.csv"; //output movies
		expressionList = { "\\\"?((.+)\\\"|.+)\\s(\\((\\d{4})|\\([?]{4})","\\((TV|V|VG)\\)","\\{\\{SUSPENDED\\}\\}","\\{(.+)?\\(\\#(\\d{1,4}).(\\d{1,5})\\)\\}","[^(]\\s(([\\d]{4})-?([\\d]{4})?)\\s?\\n" }; //movies
		//									Title,								Type,				Suspended,						episode,										Year
		break;
	case countries:
		inputFileName = "countries.list"; //input countries
		outputFileName = "countries.csv"; //output countries
		expressionList = { "\\\"?((.+)\\\"|.+)\\s(\\((\\d{4})|\\([?]{4})","\\{(.+)?\\(\\#(\\d{1,4}).(\\d{1,5})\\)\\}", "\s(\w+)\n"}; //countries
		//									Title,									episode,							  Country
		break;
	case genres:
		inputFileName = "genres.list"; //input genres
		outputFileName = "genres.csv"; //output genres
		expressionList = { "","","" }; //genres
		//1=
		//2=
		//3=
		//4=
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
			for (auto exp : expressionList) { //foreach expression in expression list => apply expression on line
				if (regex_search(line, match, regex(exp)))
					output += match.str(1) + seperator;

				else
					output += "NULL" + seperator;
			}

			//STRING COUNT
			if (stringCount(output, "NULL") != expressionList.size()) { //if output only contains null => skip
				outputFile << output << endl;
			}
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