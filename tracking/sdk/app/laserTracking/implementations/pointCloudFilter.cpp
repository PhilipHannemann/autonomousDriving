

#include "..\headers\pointCloudFilter.h"


PointCloudFilter::PointCloudFilter(double maxDistanceToOrigin, double maxDistanceBetweenPoints) {
	this->maxDistanceToOrigin = maxDistanceToOrigin;
	this->maxDistanceBetweenPoints = maxDistanceBetweenPoints;
}

PointCloud PointCloudFilter::filterPointCloud(PointCloud pointCloud) {
	return culling(pointCloud);
}

/*
	This method is for defining the biggest object inside of the given bounds
*/

PointCloud PointCloudFilter::culling(PointCloud pointCloud) {
	PointCloud resultingPointCloud;

	for (int i = 0; i < pointCloud.size(); i++) {
		Point point = pointCloud.getPointAtIndex(i);
		double radius = point.getDistance();
		if (radius <= maxDistanceToOrigin)
			resultingPointCloud.addPoint(point);
	}

	std::vector<PointCloud> objects = defineObjects(resultingPointCloud);
	resultingPointCloud = PointCloud();

	for (int i = 0; i < objects.size(); i++) {
		int pointCount = objects[i].size();
		if (pointCount > 10 && pointCount < 100) {
			if (resultingPointCloud.size() < pointCount) {
				resultingPointCloud = objects[i];
			}
		}
	}

	return resultingPointCloud;
}

/*
	This method is for defining objects for the given bounds
*/

std::vector<PointCloud> PointCloudFilter::defineObjects(PointCloud pointCloud) {
	std::vector<PointCloud> objects;
	if (pointCloud.size() == 0) return objects;

	PointCloud object;
	object.addPoint(pointCloud.getPointAtIndex(0));

	bool first = false;
	int noEnd = 0;

	for (int i = 0; i < pointCloud.size() + noEnd; i++) {
		int nextPos = (i + 1) % pointCloud.size();
		int pos = i % pointCloud.size();
		Point p1 = pointCloud.getPointAtIndex(pos);
		Point p2 = pointCloud.getPointAtIndex(nextPos);
		double distance = p1.getDistance(p2);
		if (abs(distance) < 0.00000001) continue;

		if (distance > maxDistanceBetweenPoints && object.size() != 0) {
			objects.push_back(object);

			object = PointCloud();
		}
		else {
			if (i >= pointCloud.size() - 1 && noEnd < pointCloud.size() && object.size() <= pointCloud.size()) {
				noEnd++;
			}
		}
		object.addPoint(p2);
	}

	objects.push_back(object);

	return objects;
}


