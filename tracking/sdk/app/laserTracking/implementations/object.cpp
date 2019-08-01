
#include "..\headers\object.h"


Object2D::Object2D() {
	contour = new RectangleContour(0.18, 0.61);
}

Object2D::Object2D(std::string str) {
	contour = new RectangleContour(0.18, 0.61);
	if (str == std::string("w")) {
		this->preSestPose = Pose(1.0, 1.0, M_PI_2);
	}
	else if (str == std::string("d")) {
		this->preSestPose = Pose(1.0, 1.0, M_PI_4);
	}
	else if (str == std::string("-d")) {
		this->preSestPose = Pose(1.0, 1.0, -M_PI_4);
	}
}

Object2D::Object2D(IContour *contour) {
	this->contour = contour;
}

void Object2D::transform(Pose pose) {

	this->pose.x += pose.x;
	this->pose.y += pose.y;
	this->pose.phi += pose.phi;

	this->pose.phi -= this->pose.phi > 2*M_PI ? 2*M_PI : 0.0;
	this->pose.phi += this->pose.phi < -2*M_PI ? 2*M_PI : 0.0;
	contour = contour->transform(pose);
}

double Object2D::matchToPointCloud(PointCloud pointCloud, Pose &pose) {
	double error = 1e10;
	Point m = pointCloud.getMiddlePoint();
	if (this->preSestPose == Pose()) {
		for (double phi = -M_PI; phi < M_PI; phi += M_PI_2) {
			Pose tryPose = Pose(m.x * 2, m.y * 2, phi);
			double tryError = Optimizer::fitOntoEachOther(pointCloud, contour, &tryPose);
			if (tryError < error) {
				pose = tryPose;
			}
		}
		preSestPose = pose;
	}
	else {
		preSestPose.y += 0.1;
		error = Optimizer::fitOntoEachOther(pointCloud, contour, &preSestPose);
		pose = preSestPose;
	}
	
	return error;
}

Pose Object2D::getPose() {
	return pose;
}


IContour *Object2D::getContour() {
	
	IContour *contour = this->contour->getCopy();
	return contour;
}

void Object2D::setContour(IContour *contour) {
	this->contour = contour;
}