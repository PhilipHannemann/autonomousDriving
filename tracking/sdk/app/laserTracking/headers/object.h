
#pragma once

#include "types.h"
#include "pointCloud.h"
#include "contours.h"
#include "optimizer.h"

/*
	The class Object2D is a construct, witch contains class of type IContour and 
	is meant to define an object in the 2D space.
	It's purpose is to be located inside of PointCloud defined by a sensor
*/
class Object2D {
	private:
		Pose pose;
		IContour *contour;
		Pose preSestPose;

	public:
		Object2D();
		Object2D::Object2D(std::string str);
		Object2D(IContour *contour);
		void transform(Pose pose);
		double matchToPointCloud(PointCloud pointCloud, Pose &pose);
		Pose getPose();
		IContour *getContour();
		void setContour(IContour *contour);
};
