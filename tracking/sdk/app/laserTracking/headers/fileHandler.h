
#include <iostream>
#include <fstream>
#include <string>

#pragma once

/*
	The FileHandler class is an absraction level for saving a file.
	All you have to do for writing a file is, to 
	- set the foldername (if not it is the current workspace directory)
	- open a file with a specified name
	- and then you can append as often as you want strings to it
	- last you should close the file --> now it is saved
*/

class FileHandler {
private:
	std::ofstream file;
	std::string folderName;
public:
	void open(std::string fileName);
	void setFolderName(char *folderName);
	void setFolderName(std::string folderName);
	void close();
	void appendToFile(std::string string);
};
