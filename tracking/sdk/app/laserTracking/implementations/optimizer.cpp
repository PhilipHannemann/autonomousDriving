
#include "..\headers\optimizer.h"

#define _USE_MATH_DEFINES
#include <math.h>


using namespace alglib;


inline void getOriginDirectionError(IContour *contour, PointCloud pointCloud, DistanceMap map, real_1d_array &fi, Pose pose) {
	Point *points = pointCloud.getPoints();

	for (int i = 0; i < pointCloud.size(); i++) {
		if (i == 20)
			i = i;
		Point p = points[i];
		Line line = map.closestLine[i];
		
		Vector normal = line.getNormalVector();
		Vector v(p.x, p.y);
		double angle = normal.angleTo(v);
		double cost = 0.0;
		if (abs(angle) < M_PI_2){
			if (angle > 0)
				cost = M_PI_2 - angle;
			else
				cost =  M_PI_2 + angle;
		}
		
		fi[i + pointCloud.size()] = cost;
	}
	//printf("\n\n");
}

inline void optimizationFunction(const real_1d_array &x, real_1d_array &fi, void *ptr){

	PointCloud pointCloud = Optimizer::getPointCloud();
	IContour *contour = Optimizer::getContour();
	Pose pose = Pose(x[0], x[1], x[2]);

	IContour *tContour = contour->transform(pose);
	double sum = 0.0;

	DistanceMap map = tContour->getMinDistanceToPointCloud(pointCloud);

	for (int i = 0; i < map.size(); i++) {
		sum += pow(map.distance[i], 2);
		fi[i] = map.distance[i];
	}

	//getOriginDirectionError(contour, pointCloud, map, fi, pose);
	//fi[pointCloud.size()] = dm > dp ? dm - dp : 0.0;

	Optimizer::setError(sum);
}

double Optimizer::error;
PointCloud Optimizer::pointCloud;
IContour *Optimizer::contour;

double Optimizer::fitOntoEachOther(PointCloud pointCloud, IContour *contour, Pose *pose) {
	if (pointCloud.size() == 0) return -1;

	Optimizer::contour = contour;
	Optimizer::pointCloud = pointCloud;


	real_1d_array x = "[0.0, 0.0, 0.0]"; //[x, y, phi]

	//Point m = pointCloud.getMiddlePoint();

	if (!(*pose == Pose())){
		x[0] = pose->x;
		x[1] = pose->y;
		x[2] = pose->phi;
	}
	

	double epsx = 0.00001;
	ae_int_t maxits = 0;
	minlmstate state;
	minlmreport rep;

	minlmcreatev(pointCloud.size(), x, 0.0001, state);
	minlmsetcond(state, epsx, maxits);
	minlmoptimize(state, optimizationFunction);
	minlmresults(state, x, rep);

	printf("\r[%.3f, %.3f, %.3f] \twith an error of: %5.6f -- \n", x[0], x[1], x[2]/M_PI *180, error); // EXPECTED: [-3,+3]

	pose->x = x[0];
	pose->y = x[1];
	pose->phi = x[2];

	return error;
}


void Optimizer::setError(double e) {
	error = e;
}

PointCloud Optimizer::getPointCloud() {
	return pointCloud;
}

IContour *Optimizer::getContour() {
	return contour;
}