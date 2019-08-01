#include "..\headers\types.h"



// TIME


Time::Time() {
	localTime = localtime(&time);
}

Time::Time(tm *lTime, int ms) {
	time = std::time(NULL);
	localTime = localtime(&time);

	milliseconds = ms;
}

char *Time::getAsChar() {
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S", localTime);
	std::string str(buffer);
	str += ":" + SSTR(milliseconds);
	char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	return cstr;
}


//TIMESTAMP


TimeStamp::TimeStamp() {

	time_t time = std::time(NULL);
	tm*localTime = localtime(&time);
	ms = SystemTime::getSystemMilliseconds();
	milliseconds1970 = (double)time*1000.0 + (double)ms;
	this->time = Time(localTime, ms);
}

TimeStamp::TimeStamp(long ms) {
	milliseconds1970 = ms;
	time_t time = ms / 1000;
	tm*localTime = localtime(&time);
	ms = ms % 1000;
	this->time = Time(localTime, ms);
}

const char *TimeStamp::AsChar() {
	return time.getAsChar();
}

bool TimeStamp::operator< (const TimeStamp &t) {
	return this->milliseconds1970 < t.milliseconds1970;
}

bool TimeStamp::operator> (const TimeStamp &t) {
	return this->milliseconds1970 > t.milliseconds1970;
}


// DOUBLE


bool Double::isInBetween(double d1, double m, double d2) {
	double smallVal = d1 < d2 ? d1 : d2;
	double heighVal = d1 < d2 ? d2 : d1;

	if (smallVal <= m && m <= heighVal) return true;
	return false;
}


// POSE


Pose::Pose(double x, double y, double phi) {
	this->x = x;
	this->y = y;
	this->phi = phi;
}

Pose::Pose() {
	this->x = 0.0;
	this->y = 0.0;
	this->phi = 0.0;
}

std::string Pose::getAsCSV() {
	std::string csv = "";
	csv += SSTR(x) + ";" + SSTR(y) + ";" + SSTR(phi / M_PI * 180) + "\n";
	return csv;
}

bool Pose::operator== (const Pose &p) {
	if (x != p.x) return false;
	if (y != p.y) return false;
	if (phi != p.phi) return false;
	return true; 
}


// POINT


Point::Point(double x, double y) {
	this->x = x;
	this->y = y;
}
Point::Point() {
	x = 0.0;
	y = 0.0;
}

double Point::getDistance(Point point) {
	return sqrt(pow((x - point.x), 2) + pow((y - point.y), 2));
}

double Point::getDistance() {
	return sqrt(pow((x), 2) + pow((y), 2));
}

Point Point::transform(Pose pose) {
	Point p;
	p.x = pose.x + x * cos(pose.phi) - y * sin(pose.phi);
	p.y = pose.y + x * sin(pose.phi) + y * cos(pose.phi);
	return p;
}


// VECTOR


Vector::Vector(double dx, double dy) {
	this->dx = dx;
	this->dy = dy;
}

Vector::Vector() {
	dx = 0.0;
	dy = 0.0;
}

void Vector::norm() {
	double d = lenght();

	dx = dx / d;
	dy = dy / d;
}

double Vector::lenght() {
	return sqrt(pow((dx), 2) + pow((dy), 2));
}

void Vector::rotate90() {
	double y = dx * (-1);
	dx = dy;
	dy = y;
}

double Vector::angleTo(Vector v) {
	double a1 = atan2(v.dy, v.dx);
	double a2 = atan2(dy, dx);
	double da = a2 - a1;
	while (da > M_PI) da -= 2 * M_PI;
	while (da < -M_PI) da += 2 * M_PI;

	return da;
}


// LINE


Line::Line(Point p1, Point p2) {
	this->p1 = p1;
	this->p2 = p2;
}
Line::Line() {}

double Line::getDistance(Point point) {
	Pose pose;

	Vector v1;
	v1.dx = p2.x - p1.x;
	v1.dy = p2.y - p1.y;

	pose.phi = -atan2(v1.dy, v1.dx);

	Line nLine = transform(pose);
	Point nPoint = point.transform(pose);

	bool insight = Double::isInBetween(nLine.p1.x, nPoint.x, nLine.p2.x);
	if (insight) return nPoint.y - nLine.p2.y;

	double delta = nPoint.y - nLine.p2.y;
	double sign;
	if (abs(delta) < 0.0000001) sign = 1.0;
	else sign = delta >= 0.0 ? 1 : -1;

	double d1 = nPoint.getDistance(nLine.p1);
	double d2 = nPoint.getDistance(nLine.p2);

	return d1 < d2 ? d1 * sign : d2 * sign;
}

Line Line::transform(Pose pose) {
	Point p1 = this->p1.transform(pose);
	Point p2 = this->p2.transform(pose);
	return Line(p1, p2);
}

Vector Line::getNormalVector() {
	Vector vector(p2.x - p1.x, p2.y - p1.y);
	vector.rotate90();
	vector.norm();
	return vector;
}


// SIDE


Side::Side() {

}

Side::Side(Point points[]) {
	this->points = std::vector<Point>(points, points + sizeof points / sizeof points[0]);
}

Side::Side(std::vector<Point> points) {
	this->points = points;
}

void Side::addPoint(Point p) {
	points.push_back(p);
}

Side Side::transform(Pose pose) {
	std::vector<Point> points;
	for (int i = 0; i < this->points.size(); i++) {
		points.push_back(this->points[i].transform(pose));
	}

	return Side(points);
}

double Side::getMinDistanceToPoint(Point point) {
	double minDistance;
	for (int i = 0; i < points.size() - 1; i++) {
		int nextPoint = i + 1;
		Line line = Line(points[i], points[nextPoint]);

		double d = line.getDistance(point);
		if (i == 0 || minDistance > d) {
			minDistance = d;
			lastUsedLine = line;
		}
	}
	return minDistance;
}


Point Side::getMiddlePoint() {
	Point middle;
	for (int i = 0; i < points.size(); i++) {
		middle.x += points[i].x;
		middle.y += points[i].y;
	}
	middle.x /= points.size();
	middle.y /= points.size();
	return middle;
}


// DISTANCEMAP


DistanceMap::DistanceMap() {
	mostUsedSide = 0;
}

double DistanceMap::size() {
	return distance.size();
}

void DistanceMap::addDistance(double d) {
	distance.push_back(d);
}

void DistanceMap::addLine(Line l) {
	closestLine.push_back(l);
}


// SIDECOUNTER


SideCounter::SideCounter(int sideCount) {
	sideIDs = new int[sideCount];
	this->sideCount = sideCount;
	for (int i = 0; i < sideCount; i++) {
		sideIDs[i] = 0;
	}
}

void SideCounter::incrementCountForID(int id) {
	if (id < sideCount && id >= 0) {
		sideIDs[id]++;
	}
}

int SideCounter::getMostUsedSideID() {
	int maxCount = 0;
	int mostUsedSide = 0;

	for (int i = 0; i < sideCount; i++) {
		if (sideIDs[i] > maxCount) {
			maxCount = sideIDs[i];
			mostUsedSide = i;
		}
	}

	return mostUsedSide;
}

