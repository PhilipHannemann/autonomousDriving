#include <stdio.h>
#include <stdlib.h>

#pragma once
#include "headers/tracking.h"
#include "headers/thread.h"
#include "headers/test.h"
#include "headers/terminal.h"
#include "headers/lighthouse.h"


std::vector<std::string> split(std::string str, std::string delimiter) {
	std::vector<std::string> substrings;
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		token = str.substr(0, pos);
		substrings.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	if (str.size() != 0) {
		substrings.push_back(str);
	}
	return substrings;
}

/*
	starting point of the program
*/

int main(int argc, const char *argv[]) {
	
	Terminal terminal(argc, argv);

	terminal.addParameter(Param('t', std::string("TRACKER"), true));
	terminal.addParameter(Param('s', std::string("SYNCHRONOUS")));
	terminal.addParameter(Param('o', std::string("OBJECT"), true));
	terminal.addParameter(Param('p', std::string("PATH"), true));
	terminal.addParameter(Param('c', std::string("COMPORT"), true));
	terminal.addParameter(Param('v', std::string("VALIDATE")));

	std::vector<Param> parameters = terminal.parseArguments();
	


	std::vector<std::string> definedTrackers = split(parameters[0].argument, "&");

	IPoseSensor **trackers = new IPoseSensor*[definedTrackers.size()];

	for (int i = 0; i < definedTrackers.size(); i++) {
		if (definedTrackers[i] == std::string("t"))
			trackers[i] = new PoseTracker(new TestSensor(), Object2D(parameters[2].argument), parameters[3].argument);
		if (definedTrackers[i] == std::string("r"))
			trackers[i] = new PoseTracker(new RPLidar(parameters[4].argument), Object2D(parameters[2].argument), parameters[3].argument);
		if (definedTrackers[i] == std::string("l"))
			trackers[i] = new Lighthouse(2, parameters[3].argument);
	}
	

	if(parameters[5].argument.size() > 0)
		Test();

	if (definedTrackers.size() == 0)
		return 0;

	Tracking tracking = Tracking(trackers, definedTrackers.size());

	/*IPoseSensor **trackers = new IPoseSensor*[1];
	trackers[0] = new PoseTracker(new TestSensor(), Object2D(""), "");
	Tracking tracking = Tracking(trackers, 1);*/
	tracking.start();
	

	getchar();

	return 0;
}
