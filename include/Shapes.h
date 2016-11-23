#pragma once
#include <math.h>
#include <glut.h>

#define PI 3.14159265359f

// Colors
#define BROWN { 0.45f, 0.29f, 0.07f, 1.0f }
#define YELLOW { 1.0f, 1.0f, 0.0f }
#define WHITE { 1.0f, 1.0f, 1.0f }

struct Point {
	double x, y, z;
};

struct Poly {
	Point p1, p2, p3, p4;
};

struct Triangle {
	Point p1, p2, p3;
};


struct Path {
	Point *p1, *p2, *p3, *p4;
};

void drawCylinder(float pos, float rad1, float rad2, float height, int slices);

void drawDisk(float pos, float rad1, float rad2);

void drawSphere(float pos, float rad);

void drawTorus(float pos, float rad1, float rad2);

void drawPoly(Poly r, float texRep);

void drawPyramid(float height, bool rotated);

void drawDiamond(float rad);

void drawWall(float roomSize, int dir, bool rotX, bool rotY, bool rotZ, bool transX, bool transY, bool transZ);

void drawScope();

Point* bezier(float t, Path* path);
