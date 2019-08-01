
#include "..\headers\rpLidarSensor.h"


using namespace rp::standalone::rplidar;

RPLidar::RPLidar() {
	baudrateArray[0] = 115200;
	baudrateArray[1] = 256000;

#ifdef _WIN32
	comPath = "\\\\.\\com4";
#else
	comPath = "/dev/ttyUSB0";
#endif

	drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
	connect();
}

RPLidar::RPLidar(std::string comPath) {
	baudrateArray[0] = 115200;
	baudrateArray[1] = 256000;

#ifdef _WIN32
	comPath = "\\\\.\\" + comPath;
#else
	comPath = "/dev/ttyUSB0";
#endif

	this->comPath = new char[comPath.length() + 1];
	strcpy(this->comPath, comPath.c_str());
	
	drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
	connect();
}

RPLidar::~RPLidar() {
	RPlidarDriver::DisposeDriver(drv);
	drv = NULL;
	delete[] comPath;
}


bool RPLidar::checkHealth() {
	laserHealth = drv->getHealth(healthinfo);

	if (IS_OK(laserHealth)) {
		printf("RPLidar health status : %d\n", healthinfo.status);

		if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
			fprintf(stderr, "Error, rplidar internal error detected. Please reboot the device to retry.\n");
			return false;
		}
		return true;
	}

	fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", laserHealth);
	return false;
}


bool RPLidar::connect() {
	if (!drv) return false;

	size_t baudRateArraySize = _countof(baudrateArray);
	rplidar_response_device_info_t devinfo;

	for (size_t i = 0; i < baudRateArraySize; ++i) {
		if (!drv)
			drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);

		if (IS_OK(drv->connect(comPath, baudrateArray[i]))) {
			laserHealth = drv->getDeviceInfo(devinfo);

			if (IS_OK(laserHealth) && checkHealth()) {
				return true;
			}

			delete drv;
			drv = NULL;
		}
	}
}


PointCloud RPLidar::waitForNextPointCloud() {
	if (!drv) return PointCloud(); 

	rplidar_response_measurement_node_t nodes[8192];
	PointCloud pointCloud;
	size_t valueCountForThisRound = _countof(nodes);
	laserHealth = drv->grabScanData(nodes, valueCountForThisRound);

	if (IS_OK(laserHealth)) {
		drv->ascendScanData(nodes, valueCountForThisRound);

		for (int i = 0; i < valueCountForThisRound; i++) {

			double radius = nodes[i].distance_q2 / 4000.0f;
			double angle = (nodes[i].angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) / 64.0f;

			pointCloud.addPolarPoint(radius, angle);
			
			//package[i].quality = nodes[i].sync_quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT;
		}

		return pointCloud;
	}

	return PointCloud();
}

PointCloud RPLidar::getLatestPointCloud() {
	return lastPointCloud;
}

size_t RPLidar::getDataCount() {
	return lastPointCloud.size();
}


void RPLidar::start() {
	if (!drv) return;

	drv->startMotor();
	//drv->startScan(0, 1);
	drv->startScanNormal(0);
}

void RPLidar::stop() {
	if (!drv) return;

	drv->stop();
	drv->stopMotor();
}