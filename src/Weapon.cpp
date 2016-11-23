#pragma once

#include "Weapon.h"

void Weapon::draw() {
	glPushMatrix();

	glTranslatef(currPos->x, currPos->y, currPos->z);
	glRotated(rotation->x, 1, 0, 0);
	glRotated(rotation->y, 0, 1, 0);
	glRotated(rotation->z, 0, 0, 1);
	glScalef(scale, scale, scale);

	glBindTexture(GL_TEXTURE_2D, texID);

	switch (type)
	{

	case BULLET:
		drawBullet(); break;

	case GRENADE:
		drawGrenade(); break;

	case SHURIKEN:
		drawShuriken(); break;
	default:
		break;
	}

	glPopMatrix();
}

void Weapon::still() {
	currPos = new Point{ 0, -2, 10};
	scale = 0.2;
	rotation = new Point{ -10, 0, 0 };
	step = 0;
}

void Weapon::move() {

	currPos->y = 0;

	switch (type)
	{
	case BULLET:
		currPos->z += 0.5;

		rotation->z += 20;
		break;

	case GRENADE:
		currPos = bezier(step, path);

		rotation->x += 10;
		rotation->y += 5;
		rotation->z += 5;
		break;

	case SHURIKEN:
		currPos = bezier(step, path);

		rotation->y += 2;
		break;

	default:
		break;
	}

}

void Weapon::animate() {
	setPath();

	if (!isFired) still();
	else move();
}

void Weapon::setPath() {

	Point *p1, *p2;

	switch (type)
	{

	case BULLET: return;

	case GRENADE:
		// Grenade
		p1 = new Point{ targetPos->x, fireHeight, targetPos->z / 4 };
		p2 = new Point{ targetPos->x, fireHeight, targetPos->z * 3 / 4 };
		break;

	case SHURIKEN:
		// Shuriken
		p1 = new Point{ fireHeight / 4, 0, targetPos->z / 2 };
		p2 = new Point{ fireHeight, fireHeight/2, targetPos->z * 4/5};
		break;
	default:
		break;
	}

	// Path
	path = new Path{ initPos, p1, p2, targetPos };

}

void Weapon::drawPath() {

	glPushMatrix();

	glPointSize(1);
	glColor3f(1, 1, 1);

	// Grenade & Shuriken
	glNormal3d(0, 0, -1);

	glBegin(GL_POINTS);

	for (float t = 0; t < 1; t += 0.001)
	{
		Point* p = bezier(t, path);
		glVertex3f(p->x, p->y, p->z);
	}
	glEnd();
	glPopMatrix();
}
