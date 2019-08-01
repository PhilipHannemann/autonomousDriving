

#pragma once
#include <string>
#include <vector>
#include <iostream>

/*
	The structure Param defines the behaviour of a given terminlal/cmd parameter 
*/
struct Param {
	Param() {}
	Param(char symbol, std::string explanation, bool needsArgument = false) {
		this->symbol = symbol;
		this->needsArgument = needsArgument;
		this->explanation = explanation;
	}
	bool needsArgument;
	char symbol;
	std::string argument;
	std::string explanation;
};

/*
	This class is meant to extract specified parameters from a given argument vector
*/
class Terminal {
	std::vector<Param> parameters;
	int argc;
	const char **argv;
public:
	Terminal(int argc, const char *argv[]);
	void addParameter(Param &param);
	std::vector<Param> parseArguments();
	void errorArgumentNeeded(Param p);
	void Terminal::help();
};