
#pragma once 

#include "sensors.h"
#include "object.h"
#include "rpLidarSensor.h"
#include "optimizer.h"
#include "fileHandler.h"
#include "pointCloudFilter.h"

/*
	The PoseSensor class is meant to calculated a pose of an specific object within a received PointCloud of a specifid IPointCloudSensor.
*/
class PoseTracker: public IPoseSensor{

		IPointCloudSensor *sensor;
		Object2D object;
		Pose pose;
		FileHandler file;
		int poseCount;

	public:
		PoseTracker(IPointCloudSensor *sensor, Object2D object, std::string path);
		PoseTracker();
		Pose getPose();
		void start();
		void stop();
};
