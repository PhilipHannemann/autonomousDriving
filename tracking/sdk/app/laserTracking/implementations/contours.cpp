

#include "..\headers\contours.h"


PolygonContour::PolygonContour(Side *sides) {
	this->sides = std::vector<Side>(sides, sides + sizeof(sides) / sizeof(sides[0]));
}

PolygonContour::PolygonContour(std::vector<Side> sides) {
	this->sides = sides;
}

DistanceMap PolygonContour::getMinDistanceToPointCloud(PointCloud pointCloud) {
	Point *points = pointCloud.getPoints();
	DistanceMap map;
	SideCounter sideCounter = SideCounter(sides.size());
	for (int i = 0; i < pointCloud.size(); i++) {
		int sideID = 0;
		double minDistance = 1e10;
		for (int ii = 0; ii < sides.size(); ii++) {
			Line l = Line(sides[ii].points[0], sides[ii].points[1]);
			Vector n = l.getNormalVector();
			Vector m = Vector((l.p1.x + l.p2.x) / 2, (l.p1.y + l.p2.y) / 2);
			double angle = n.angleTo(m);

			double tolerance = 0.0;
			if (angle <= -M_PI_2 + tolerance || angle >= M_PI_2 - tolerance)
			{
				double d = sides[ii].getMinDistanceToPoint(points[i]);
				if (d < minDistance) {
					minDistance = d;
					sideID = ii;
				}
			}
		}

		sideCounter.incrementCountForID(sideID);
		map.addDistance(minDistance);
		map.addLine(sides[sideID].lastUsedLine);
	}

	map.mostUsedSide = sideCounter.getMostUsedSideID();
	return map;
}

PolygonContour *PolygonContour::transform(Pose pose) {
	PolygonContour *contour = new PolygonContour();
	for (int i = 0; i < sides.size(); i++) {
		Side side = sides[i].transform(pose);
		contour->addSide(side);
	}
	return contour;
}

void PolygonContour::addSide(Side side) {
	this->sides.push_back(side);
}

double PolygonContour::getDistanceToOrigin(DistanceMap map) {
	Point M = sides[map.mostUsedSide].getMiddlePoint();

	return M.getDistance();
}

PolygonContour *PolygonContour::getCopy() {
	PolygonContour *contour = new PolygonContour(sides);

	return contour;
}

IContour **PolygonContour::getPartlyContours(int *count){
	IContour **contours = new IContour*[sides.size()];
	*count = sides.size();

	for (int i = 0; i < sides.size(); i++) {
		std::vector<Side> sides;
		sides.push_back(this->sides[i]);
		contours[i] = new PolygonContour(sides);
	}

	return contours;
}




RectangleContour::RectangleContour(double width, double height) {
	
	double xd = 1;
	double yd = 1;

	Point *points = new Point[4];

	for (int i = 0; i < 4; i++) {
		points[i].x = width / 2.0 *xd;
		points[i].y = height / 2.0 *yd;

		xd *= i % 2 == 0 ? -1 : 1;
		yd *= i % 2 == 0 ? 1 : -1;
	}

	for (int i = 0; i < 4; i++) {
		int nextPoint = i == 3 ? 0 : i + 1;
		Side side;
		side.addPoint(points[i]);
		side.addPoint(points[nextPoint]);

		addSide(side);
	}
}


