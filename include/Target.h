#include "Shapes.h"

struct Target {
	Point* currPos;
	float radius;
	GLuint texID;
	void draw();
};