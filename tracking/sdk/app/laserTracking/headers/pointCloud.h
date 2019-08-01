
#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include "types.h"


class PointCloud : public IMesurement{
private:
	std::vector<Point> points;

public:
	PointCloud();
	PointCloud(Point points[]);
	Point *getPoints();
	Point getClosestPoint(Point point);
	Point getMiddlePoint();
	void addPoint(Point point);
	void addPolarPoint(double radius, double angle);
	Point getOrtogonalPointForDistance(double distance);
	int size();
	Vector findV();
	Point findM();
	Point PointCloud::getPointAtIndex(int index);
	std::string getAsCSV();
};


#endif 