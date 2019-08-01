
#include "..\headers\sensors.h"


/*
	This function is for requesting the latest optained pointCloud
	@return latestPointCloud
*/
PointCloud TestSensor::getLatestPointCloud() {
	return latestPointCloud;
}

/*
	This function is for requestinga new pointCloud
	This function is waiting until a new pointCloud exists
	@return latestPointCloud
*/
PointCloud TestSensor::waitForNextPointCloud() {
	PointCloud pointCloud;
	for (double xy = 0.0; xy <= 0.5; xy += 0.01) {
		pointCloud.addPoint(Point(xy - 0.0 + offset, xy + 0.4 + offset));
	}
	offset += 0.05;
	return pointCloud;
}

/*
	for activating the sensor
*/
void TestSensor::start() {
	stopThread = false;
	thread = new Thread(this);
	thread->start();
}

/*
	for stoping the sensor
*/
void TestSensor::stop() {
	stopThread = true;
	if(thread) thread->stop();
}

/*
	a method that can be run in a Thread class
	(recieving/creating pointClouds)
*/
void TestSensor::run() {
	while(!stopThread) {
		PointCloud pointCloud;
		for (double xy = 0.0; xy <= 0.2; xy += 0.01) {
			pointCloud.addPoint(Point(xy - 0.0, xy + 0.4));
		}
		//for (double xy = 0.0; xy <= 0.05; xy += 0.01) {
		//	pointCloud.addPoint(Point(-xy - 0.0, xy + 0.2));
		//}

		latestPointCloud = pointCloud;
		Sleep(1000);
	}
	
}