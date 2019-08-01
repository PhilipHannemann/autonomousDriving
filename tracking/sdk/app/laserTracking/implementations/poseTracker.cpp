
#include "..\headers\poseTracker.h"


/*
	You can initialize the PoseTracker by giving it the nessesary components
	@param sensor - this is a IPointCloudSensor for receiving a PointCloud in witch the algorithm can find the position of the specified object
	@param object - this is the object witch the PoseTracker should find insid of the PointCloud recieved by the PointCloudSensor
*/

PoseTracker::PoseTracker(IPointCloudSensor *sensor, Object2D object, std::string path) {
	this->sensor = sensor;
	this->object = object;
	if (path.size() == 0)
		path = "csv";
	file.setFolderName(path);
	poseCount = 0;
}

/*
	This particular initializer is for testing
	It initializes with a TestSensor and a basic Object2D

*/
PoseTracker::PoseTracker () {
	this->sensor = new TestSensor();
	this->object = Object2D();
	file.setFolderName("test");
	poseCount = 0;
}

/*
	This method shoud be called in a loop for receiving constantly the newest pose of the tracked object
*/

Pose PoseTracker::getPose() {
	
	//PointCloud pointCloud = sensor->getLatestPointCloud();
	PointCloud pointCloud = sensor->waitForNextPointCloud();
	Pose pose;

	PointCloudFilter filter(1.0, 0.03);
	pointCloud = filter.filterPointCloud(pointCloud);
	
	double error = object.matchToPointCloud(pointCloud, pose);
	//pose = object.getPose();

	file.open("round"+SSTR(poseCount++)+".csv");
	file.appendToFile(pointCloud.getAsCSV());
	file.appendToFile(pose.getAsCSV());
	file.close();
	return pose;
}


/*
	genaral ISensor mathod for activating the specified sensor
*/

void PoseTracker::start() {
	sensor->start();
}

/*
	genaral ISensor mathod for deactivating the specified sensor
*/
void PoseTracker::stop() {
	sensor->stop();
}