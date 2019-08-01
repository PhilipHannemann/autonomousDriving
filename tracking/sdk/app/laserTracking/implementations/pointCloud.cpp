
#include "..\headers\pointCloud.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <sstream>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


PointCloud::PointCloud() {

}


PointCloud::PointCloud(Point points[]) {
	this->points = std::vector<Point>(points, points + sizeof(points) / sizeof(Point));
}

int PointCloud::size() {
	return points.size();
}

Point *PointCloud::getPoints() {
	Point *points = new Point[size()];

	std::copy(this->points.begin(), this->points.end(), points);

	return points;
}

void PointCloud::addPoint(Point point) {
	points.push_back(point);
}

void PointCloud::addPolarPoint(double radius, double angle) {
	Point point;

	point.x = radius * cos(angle / 180.0 * M_PI);
	point.y = radius * sin(angle / 180.0 * M_PI);

	points.push_back(point);
}

Point PointCloud::getClosestPoint(Point point) {

	if (size() == 0) return Point();

	double closestDistance = point.getDistance(points[0]);
	Point closestEstimate = points[0];

	for (int i = 1; i < size(); i++) {
		double distance = point.getDistance(points[i]);
		if (distance < closestDistance) {
			closestEstimate = points[i];
			closestDistance = distance;
		}
	}

	return closestEstimate;
}

Point PointCloud::getMiddlePoint() {
	Point middle = Point();

	for (std::vector<Point>::size_type i = 0; i < points.size(); i++) {
		middle.x += points[i].x;
		middle.y += points[i].y;
	}

	middle.x /= (double)points.size();
	middle.y /= (double)points.size();
	return middle;
}


Point PointCloud::getOrtogonalPointForDistance(double distance) {
	Point M = findM();
	Vector v = findV();

	v.rotate90();
	v.norm();

	Point middle = getMiddlePoint();
	Point _v = Point(-1 * v.dx, -1 * v.dy);
	Point A = Point(middle.x + v.dx *distance, middle.y + v.dy*distance);
	Point _A = Point(middle.x + _v.x *distance, middle.y + _v.y*distance);

	double d1 = A.getDistance();
	double d2 = _A.getDistance();

	if (d2 > d1) {
		A = _A;
	}

	return A;
}

Vector PointCloud::findV() {
	if (points.size() == 0) return Vector();
	Point middle = getMiddlePoint();
	double closestDistance = middle.getDistance(points[0]);
	int closestEstimate = 0;

	for (int i = 1; i < points.size(); i++) {
		double distance = middle.getDistance(points[i]);
		if (distance < closestDistance) {
			closestEstimate = i;
			closestDistance = distance;
		}
	}

	Vector v = Vector();
	int c = 0;

	for (int i = closestEstimate - 10; i <= closestEstimate + 10; i += 5) {
		if (i > 0 && i < points.size() && i != closestEstimate) {
			double direction = closestEstimate > i ? -1 : 1;

			v.dx += direction * (points[i].x - points[closestEstimate].x);
			v.dy += direction * (points[i].y - points[closestEstimate].y);

			c++;
		}
	}

	v.dx /= (double)c;
	v.dy /= (double)c;

	return v;
}


Point PointCloud::findM() {
	if (points.size() == 0) return Point();

	Point middle = getMiddlePoint();
	double closestDistance = middle.getDistance(points[0]);
	Point closestEstimate = points[0];

	for (int i = 1; i < points.size(); i++) {
		double distance = middle.getDistance(points[i]);
		if (distance < closestDistance) {
			closestEstimate = points[i];
			closestDistance = distance;
		}
	}

	return closestEstimate;
}

Point PointCloud::getPointAtIndex(int index) {
	if (index < 0) {
		index = points.size() + index;
	}
	if (index >= points.size() || index < 0) {
		return Point();
	}

	return points[index];
}


std::string PointCloud::getAsCSV() {
	std::string csv = "";
	for (int i = 0; i < points.size(); i++) {
		csv += SSTR(points[i].x)+";"+SSTR(points[i].y)+"\n";
	}
	return csv;
}