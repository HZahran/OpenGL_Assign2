#include "Shapes.h"

//Cam Points
struct Camera {
	Point* org; // Eye Point
	Point* ref; // Ref Point
	float angle; // Cam Angle

	void setup();
	void reset();
};