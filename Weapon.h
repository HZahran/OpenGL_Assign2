#pragma once
#include "Bullet.h"
#include "Grenade.h"
#include "Shuriken.h"

#define BULLET 0
#define GRENADE 1
#define SHURIKEN 2

struct Weapon {
	int type; // 0 Bullet , 1 Grenade , 2 Shuriken
	bool isFired;
	Point* currPos; // Current Point
	Point* targetPos; // Target Point
	Point* initPos; // Initial Point
	Point* rotation; // Rotation Params
	float fireHeight;
	Path* path; // Weapon Bezier
	float scale;
	float step; // t of bezier
	GLuint texID;
	void draw();

	void still();

	void move();

	void animate();

	void setPath();
	
	void drawPath();
};

