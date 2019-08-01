
#pragma once
#include "..\headers\sensors.h"
#include "..\openvr\Matrices.h"
#include "..\openvr\openvr.h"
#include "..\headers\fileHandler.h"

class Lighthouse: public IPoseSensor {
private:
	vr::IVRSystem *m_pHMD;
	Matrix4 offsetMatrix;
	double device;
	std::string path;
	FileHandler file;
	int poseCount;

	double* getEuler(vr::HmdMatrix34_t posMatrix);
	Matrix4 getTransformMatrix(double* values);
	double correctAngle(double angle);
	Pose vectorToPose(double *vector);

public:
	~Lighthouse();
	Lighthouse(double device, std::string path);
	Pose getPose();
	void start() {}
	void stop() {}
};
