#include "..\headers\test.h"

/*
	starting point for testing
*/
Test::Test(){
	initRandom();
	testTypes();
	testTimeStamps();
}

/*
	initialize the randimize function with a seed	
*/
void Test::initRandom() {
	srand(TimeStamp().milliseconds1970);
}

/*
	returns a random point
	x and y coordinates can differ between -10.0 and 10.0
*/
Point getRandomPoint() {
	double r1 = (double)(rand() % 200)/10.0 - 10.0;
	double r2 = (double)(rand() % 200)/10.0 - 10.0;
	Point p = Point(r1, r2);
	return p;
}

/*
	it is the general method for testing all the types in the file types.h 
*/
void Test::testTypes() {
	testLines();
}

/*
	testing everything what is of type Line
*/
void Test::testLines() {
	testLineInit();
	testInsightLineEndings();
	testOutsightLineEndings();
	testNormalVectors();
	testLineTransform();
}

/*
	This function is for testing the getDistance function of the type Line 
	In particular this method tests points that are inside of the linebounds.
	It defines random Points with an angle of 45°.
	The third point is of a random distance to the middle point of the line and ortogonal to the line.
*/
void Test::testInsightLineEndings() {
	bool allFine = true;
	for (int i = 0; i < 20; i++) {
		Point p1 = getRandomPoint();
		double r = (double)((rand() % 200) - 100) / 10.0;
		Point p2 = Point(p1.x + r, p1.y + r);
		double d = (double)(rand() % 100) / 10.0;
		double delta = sqrt(pow(d, 2) / 2.0);

		Point p = Point(((p1.x + p2.x) / 2.0) + delta, ((p1.y + p2.y) / 2.0) - delta);

		if (r > 0){
			d *= -1;
		}

		Line line = Line(p1, p2);
		double distance = line.getDistance(p);

		if (abs(d - distance) > 0.000001) {
			std::cout << "Line.getDistance :: error | inside | predicted: " << d << " outcome: " << distance << std::endl;
			printf("--> Line(Point(%.4f, %.4f), Point(%.4f, %.4f)) <--> Point(%.4f, %.4f) \n\n", p1.x, p1.y, p2.x, p2.y, p.x, p.y);
			allFine = false;
		}
	}

	if (allFine) {
		std::cout << "Line.getDistance | inside :: ok" << std::endl;
	}
}

/*
	This function is for testing the getDistance function of the type Line
	In particular this method tests points that are outside of the linebounds.
	It defines random Points with an angle of 45°.
	The third point is of a random distance to one of the line points of the line and ortogonal to the line.
*/
void Test::testOutsightLineEndings() {
	bool allFine = true;
	for (int i = 0; i < 20; i++) {
		Point p1 = getRandomPoint();
		double r = (double)((rand() % 200) - 100.0) / 10.0;
		Point p2 = Point(p1.x + r, p1.y + r);
		double d = (double)(rand() % 100) / 10.0;
		double delta = sqrt(pow(d, 2) / 2.0);

		Point p;
		if (r > 0) {
			p = Point(p1.x - delta, p1.y - delta);
		}
		else {
			p = Point(p1.x + delta, p1.y + delta);

		}


		Line line = Line(p1, p2);
		double distance = line.getDistance(p);

		if (abs(d - distance) > 0.000001) {
			std::cout << "Line.getDistance :: error | outside | predicted: " << d << " outcome: " << distance << std::endl;
			printf("--> Line(Point(%.4f, %.4f), Point(%.4f, %.4f)) <--> Point(%.4f, %.4f) \n\n", p1.x, p1.y, p2.x, p2.y, p.x, p.y);
			allFine = false;
		}
	}

	if (allFine) {
		std::cout << "Line.getDistance | outside :: ok" << std::endl;
	}
}


void Test::testNormalVectors() {
	bool allFine = true;
	for (int i = 0; i < 20; i++) {
		Point p1 = getRandomPoint();
		double r = (double)((rand() % 100)) / 10.0;
		Point p2 = Point(p1.x + r, p1.y + r);
		Line line(p1, p2);
		double delta = sqrt(pow(1.0, 2) / 2.0);

		Vector v = line.getNormalVector(), predictedV(delta,  -delta);


		if (abs(v.dx - predictedV.dx) > 0.000001) {
			std::cout << "Line.getNormalVector :: error | dx | predicted: " << predictedV.dx << " outcome: " << v.dx << std::endl;
			allFine = false;
		}
		if (abs(v.dy - predictedV.dy) > 0.000001) {
			std::cout << "Line.getNormalVector :: error | dy | predicted: " << predictedV.dy << " outcome: " << v.dy << std::endl;
			printf("--> Line(Point(%.4f, %.4f), Point(%.4f, %.4f)) \n\n", p1.x, p1.y, p2.x, p2.y);
			allFine = false;
		}
	}

	if (allFine) {
		std::cout << "Line.getNormalVector :: ok" << std::endl;
	}
}


void Test::testLineInit() {
	bool allFine = true;
	for (int i = 0; i < 20; i++) {
		Point p1 = getRandomPoint();
		Point p2 = getRandomPoint();
		Line l(p1, p2);

		if (p1.x != l.p1.x || p1.y != l.p1.y || p2.x != l.p2.x || p2.y != l.p2.y) {
			allFine = false;
			std::cout << "Line.init :: error | points differ | compare: " << std::endl;
			printf("--> %f==%f | %f==%f | %f==%f | %f==%f | \n\n", p1.x, l.p1.x, p1.y, l.p1.y, p2.x, l.p2.x, p2.y, l.p2.y);
		}
	}
	
	if (allFine) {
		std::cout << "Line.init :: ok" << std::endl;
	}
}

void Test::testLineTransform() {
	bool allFine = true;
	for (int i = 0; i < 20; i++) {
		Point p1 = getRandomPoint();
		double r = (double)((rand() % 200) - 100) / 10.0;
		Point p2 = Point(p1.x + r, p1.y + r);

		Line line = Line(p1, p2);
		line = line.transform(Pose(0, 0, -M_PI_4));

		if (abs(line.p2.y-line.p1.y) > 0.000001) {
			std::cout << "Line.transform :: error" << std::endl;
			printf("--> Line(Point(%.4f, %.4f), Point(%.4f, %.4f))\n\n", line.p1.x, line.p1.y, line.p2.x, line.p2.y);
			allFine = false;
		}
	}

	if (allFine) {
		std::cout << "Line.transform :: ok" << std::endl;
	}
}


void Test::testTimeStamps() {
	TimeStamp timeStart;
	for (int i = 0; i < 10000; i++) {
		TimeStamp timeStamp = TimeStamp();
		if (timeStamp > TimeStamp()){
			printf("TimeStamp.<> :: error");
			break;
		}
	}
	Sleep(1);
	if (timeStart<TimeStamp()) {
		printf("TimeStamp.<> :: ok");
	}
	else {
		printf("TimeStamp.<> :: error");
	}
}