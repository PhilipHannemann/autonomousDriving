#include "..\alglib\optimization.h"

#include "types.h"
#include "contours.h"
#include "pointCloud.h"

#pragma once

using namespace alglib;

/*
	The Optimizer class is an abstraction level of the algLib library.
	It is meant to find the optimal position(pose) for a Contour placed inside a PointCloud.
	And the main function for getting the obtimum is fitOntoEachOther.
	The entire class is static
*/

class Optimizer{
	private:
		static double error;
		static PointCloud pointCloud;
		static IContour *contour;

	public:
		/*
			The function fitOntoEachOther is the main function witch you give:
			@param pointCloud - the points onto witch the contour should be placed.
			@param contour - the objects appearence
			@param pose - this is a call by reference value, waht is beeing changed by the algorithm 
				and the resulting pose what should be the optimal posion and rotation for the contour
			@return the return value is the resultign error of the calculation. It is only positiv and the closer it gets to 0 the better.
		*/
		static double fitOntoEachOther(PointCloud pointCloud, IContour *contour, Pose *pose);
		static void setError(double error);
		static PointCloud getPointCloud();
		static IContour *getContour();
};