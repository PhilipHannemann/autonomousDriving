
#pragma once

#ifndef TYPES_H
#define TYPES_H

#define _USE_MATH_DEFINES
#include <math.h>

#include <vector>
#include <sstream>
#include <ctime>
#include <time.h>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()


#ifdef _WIN32
#include "windows.h"
struct SystemTime {
	static int getSystemMilliseconds() {
		SYSTEMTIME time;
		GetSystemTime(&time);
		return (int)time.wMilliseconds;
	}
};
#else

#include <sys/time.h>
	static int getSystemMilliseconds() {
		struct timeval time;
		gettimeofday(&time, NULL);
		long millis = (time.tv_sec * 1000) + (time.tv_usec / 1000);
		return time.tv_usec / 1000;
	}

#endif 


/*
	The time structure gives the possibility to store a time with millisecondes 
*/

struct Time {
	time_t time;
	tm *localTime;
	int milliseconds;

	Time();
	Time(tm *lTime, int ms);
	char *getAsChar();
};

/*
	The timestamp is meant to make it easier to create and compare two different times.
*/

struct TimeStamp {
	int ms;
	Time time;
	double milliseconds1970;

	TimeStamp();
	TimeStamp(long ms);

	const char *AsChar();
	bool operator< (const TimeStamp &t);
	bool operator> (const TimeStamp &t);
};

/*
	The Double structure is providing operations for double values
*/
struct Double {
	static bool isInBetween(double d1, double m, double d2);
};

/*
	The Pose structure gives an orientation and position of an object
*/
struct Pose {
	
	double x, y, phi;
	
	Pose(double x, double y, double phi);
	Pose();

	std::string getAsCSV();
	bool operator== (const Pose &p);
};

/*
	The Point structure defines x and y coordinates for a point in a coordinate system
*/
struct Point{
	double x, y;
	
	Point(double x, double y);
	Point();

	double getDistance(Point point);
	double getDistance();
	Point transform(Pose pose);
};

/*
	The Vector structure gives a x and y direction inside of a 2d coordinate system
*/
struct Vector {
	
	double dx, dy;
	
	Vector(double dx, double dy);
	Vector();

	void norm();
	double lenght();
	void rotate90();
	double angleTo(Vector v);
};

/*
	The structure Line difines a connection between two points
*/
struct Line{

	Point p1, p2;

	Line(Point p1, Point p2);
	Line();

	double getDistance(Point point);
	Line transform(Pose pose);
	Vector getNormalVector();
};

/*
	The Side structure defines as many lines as needed to define one side of an 2d object
*/
struct Side{
  
  std::vector<Point> points;
  Line lastUsedLine;
  
  Side();
  Side(Point points[]);
  Side(std::vector<Point> points);

  void addPoint(Point p);
  Side transform(Pose pose);
  double getMinDistanceToPoint(Point point);
  Point getMiddlePoint();

};

/*
	A DistanceMap is a list of distances calculated for an array of points 
*/
struct DistanceMap {
	std::vector<double> distance;
	std::vector<Line> closestLine;
	int mostUsedSide;

	DistanceMap();

	double size();
	void addDistance(double d);
	void addLine(Line l);
};

/*
	The SideCounter provides the possibility to calculate the most called Side 
*/
struct SideCounter {
	int *sideIDs;
	int sideCount;

	SideCounter(int sideCount);

	void incrementCountForID(int id);
	int getMostUsedSideID();
};


/*
	The IMesurment interface is a return type of an abstract sensor
*/
class IMesurement {

};


#endif