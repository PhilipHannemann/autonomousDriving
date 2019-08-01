

#pragma once

#include "sensors.h"
#include <rplidar.h>
#include "types.h"
/*
	This class is for connecting a specific sensor named RPLidar for receiving a PointCloud

*/

class RPLidar :public IPointCloudSensor {

	private:
		char* comPath;
		u_result laserHealth;
		rplidar_response_device_health_t healthinfo;
		_u32 baudrateArray[2];
		rp::standalone::rplidar::RPlidarDriver* drv;
		PointCloud lastPointCloud;

		bool checkHealth();

	public:
		RPLidar();
		RPLidar(std::string comPath);
		~RPLidar();
		void start();
		void stop();
		PointCloud getLatestPointCloud();
		PointCloud waitForNextPointCloud();
		bool connect();
		size_t getDataCount();

};

