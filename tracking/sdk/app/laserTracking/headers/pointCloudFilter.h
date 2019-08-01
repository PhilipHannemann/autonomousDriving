
#pragma once
#include "pointCloud.h"
#include "types.h"

/*
	The PointCloudFilter class is meant to subtract only the points for the object what I am looking for
*/

class PointCloudFilter {
private: 
	double maxDistanceToOrigin;
	double maxDistanceBetweenPoints;
public:
	PointCloudFilter(double maxDistanceToOrigin, double maxDistanceBetweenPoints);
	PointCloud filterPointCloud(PointCloud pointCloud);
	PointCloud PointCloudFilter::culling(PointCloud pointCloud);
	std::vector<PointCloud> PointCloudFilter::defineObjects(PointCloud pointCloud);
};