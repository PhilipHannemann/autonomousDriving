
#include "..\headers\fileHandler.h"
#include <direct.h>

void FileHandler::open(std::string fileName) {
	try {
		file.open(folderName+"/"+fileName);
	}
	catch(...){
		printf("file %s couldn't be opened\n", fileName);
	}
	
}

void FileHandler::setFolderName(char *folderName) {
	_mkdir(folderName);

	this->folderName = std::string(folderName);
}

void FileHandler::setFolderName(std::string folderName) {
	_mkdir(folderName.c_str());

	this->folderName = folderName;
}

void FileHandler::close() {
	try {
		file.close();
	}
	catch(...){
		printf("file could not be closed.\n");
	}
}

void FileHandler::appendToFile(std::string string) {
	file << string;
}

