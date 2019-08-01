
#pragma once

#include "types.h"
#include "pointCloud.h"
#include "thread.h"
#include <windows.h>
#include <stdio.h>

/*
	The ISensor interface is for an abstraction level of a general sensor.
	you can activate and deactivate it.
*/
class ISensor {
  public:
    virtual void start() = 0;
    virtual void stop() = 0;
	//virtual IMesurement getLatestPointCloud() = 0;
	//virtual IMesurement waitForNextPointCloud() = 0;

};

/*
	The specialty of a IPointCloudSensor is, that you receive a list of points collected by the sensor 
	in a DataStructure called PointCloud.
	It also gives the opportunity to choose whether you want to receive instant the latest collected PointCloud (getLatestPointCloud)
	or you can wait until the next PointCloud is completed (waitForNextPointCloud)
*/
class IPointCloudSensor: public ISensor {
  public:
    virtual PointCloud getLatestPointCloud() = 0;
	virtual PointCloud waitForNextPointCloud() = 0;
};

/*
	The interface IPoseSensor is a special form of a sensor because you get a pre calculated pose of an object with getPose.

*/

class IPoseSensor: public ISensor {
  public:
    virtual Pose getPose() = 0;
};

/*
	The TestSensor class is only for testing the functionality of the program without the need of a real connected sensor
	It generates constantly a new PointCloud
	
*/
class TestSensor: public IPointCloudSensor, public Runnable {
	Thread *thread;
	bool stopThread;
	PointCloud latestPointCloud;
	double offset;

public:
	TestSensor() {
		offset = 0.0;
	}
	PointCloud getLatestPointCloud();
	PointCloud waitForNextPointCloud();
	void start();
	void stop();
	void run();
};