

#pragma once

#include <cstdlib> 
#include <ctime> 
#include "types.h"
#include <iostream>

/*
	This Test class is meant to test all the major functionalities of the project.
*/
class Test {

  public:
    Test();
	void initRandom();
	void testLines();
	void testInsightLineEndings();
	void testOutsightLineEndings();
	void testNormalVectors();
	void testTimeStamps();
	void testTypes();
	void testLineInit();
	void testLineTransform();
};

