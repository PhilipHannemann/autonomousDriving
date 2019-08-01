
#pragma once

#include "types.h"
#include "pointCloud.h"

/*
	The interface IContour is meant to give an abstraction level of a general Contour for a Object2D
*/
class IContour {
  public:
	/*
		getMinDistanceToPointCloud is a method that should calculate the minimal distances to every given point in the PointCloud
		@param pointCloud is the general PointCloud received by the sensor
		@return DistanceMap is a structure for saving every distance between Point and Contour + to most used side of the Contour
	*/
    virtual DistanceMap getMinDistanceToPointCloud(PointCloud pointCloud) = 0;
	/*
		The Contour shoud be transformable for a given pose
		@param pose witch the contour should be translated and rotated
		@return IContour is the new transformed contour (the object itself is not being transformed)
	*/
    virtual IContour *transform(Pose pose) = 0;
	/*
		getDistanceToOrigin is a method that gives the distance of the contour to its origin. And because the countour is 
		probably not symetric, it shoud give the most likely distance related to the points of the sensor.
		For calculating the distance to its origin a precalculated DistanceMap is nessesary witch can contain a most used sideid.
	*/
	virtual double getDistanceToOrigin(DistanceMap map) = 0;
	/*
		Because the entire Contour has to be capsulated into a pointer, it makes it a lot easier to copy the Contour with a specified function, what
		returns a copy (a new pointer) of the existing contour
	*/
	virtual IContour *getCopy() = 0;
	/*
		For precalculations of the pose sometimes it can be helpfull to only have a part of the existing contour.
		@param is call by reference and gives you later on the number of partly contours you received
		@return you receive a list of IContours 
	*/
	virtual IContour ** getPartlyContours(int *count) = 0;
};

/*
	The class PolygonContour inherits all the functionality of the interface IContour
	It is meant to be defined by lists of points conected by lines.
	To have an easy way to calculate partly objects, every part is capsuled into a Side class.
	For a rectangular class there are for example 4 sides.
*/

class PolygonContour: public IContour{
  private:
    std::vector<Side> sides;

  public:
    PolygonContour(Side *sides);
	PolygonContour(std::vector<Side> sides);
	DistanceMap getMinDistanceToPointCloud(PointCloud pointCloud);
	PolygonContour *transform(Pose pose);
	double getDistanceToOrigin(DistanceMap map);
	PolygonContour *getCopy();
	IContour ** getPartlyContours(int *count);
  protected:
	  /*
		These functions are provided for the classes inheriting from this class
	  */
	PolygonContour() {};
	void addSide(Side side);
};

/*
 *	The RectangularContour is a special PolygonContour because it has parallel sides and in every case 4 of them.
 */

class RectangleContour: public PolygonContour{
  public:
	  /*
		You can create a RectangularContour by simply giving it the width and height of the object
		By default the objects origin is in the middle of these two dimentions.
		@param width is the extention in x direction
		@param height is the extention in y direction
	  */
    RectangleContour(double width, double height);

};
