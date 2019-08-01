
#include "..\headers\terminal.h"

/*
	Initialize the terminla with the given parameters from the main() funtion
*/
Terminal::Terminal(int argc, const char *argv[]) {
	this->argc = argc;
	this->argv = argv;
}

/*
	Defining nessesary parameters for the program
*/
void Terminal::addParameter(Param &param) {
	parameters.push_back(param);
}


/*
	find the given parameters in the argument vector and extract the parameters
*/
std::vector<Param> Terminal::parseArguments() {
	if (argc < 2) {
		help();
		return parameters;
	}
	for (int i = 1; i < argc; ++i) {
		for (int ii = 0; ii < parameters.size(); ii++) {
			if (std::string(argv[i]) == std::string("-") + parameters[ii].symbol) {
				if (parameters[ii].needsArgument && i + 1 < argc)
					parameters[ii].argument = argv[++i];
				else if (parameters[ii].needsArgument)
					errorArgumentNeeded(parameters[ii]);
				else
					parameters[ii].argument = "true";
			}
		}
		if (std::string(argv[i]) == std::string("-h")) {
			help();
		}
	}

	return parameters;
}

/*
	if no Argument has been given for an parameter, bus was required, 
	than this function should be called for the specific parameter
*/
void Terminal::errorArgumentNeeded(Param p) {
	std::cerr << "-" << p.symbol << " option requires one argument." << std::endl;
}


/*
	If something is wrong, or help for the parameters has been requested
*/
void Terminal::help() {
	std::cerr << "Usage: " << argv[0];
	for (int i = 0; i < parameters.size(); i++) {
		std::cerr << " -" << parameters[i].symbol << " <" << parameters[i].explanation << ">";
	}

	std::cerr << std::endl;
}


