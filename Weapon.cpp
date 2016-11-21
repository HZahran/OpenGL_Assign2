#include "Weapon.h"

void Weapon::draw() {
	glPushMatrix();

	glTranslatef(currPos->x, currPos->y, currPos->z);
	glRotated(rotation->x, 1, 0, 0);
	glRotated(rotation->y, 0, 1, 0);
	glRotated(rotation->z, 0, 0, 1);
	glScalef(scale, scale, scale);
	glColor3d(1, 1, 1);

	switch (type)
	{

	case BULLET:
		drawBullet(); break;

	case GRENADE:
		drawGrenade(); break;

	case SHURIKEN:
		drawShuriken(texID); break;
	default:
		break;
	}

	glPopMatrix();
}

void Weapon::still() {
	currPos = new Point{ 0, -2, 0.5 };
	scale = 0.2;
	rotation = new Point{ -10, 0, 0 };
	step = 0;
}

void Weapon::move() {

	currPos->y = 0;

	switch (type)
	{
	case BULLET:
		currPos->z += 2;

		rotation->z += 5;
		break;

	case GRENADE:
		currPos = bezier(step, path);

		rotation->x += 5;
		rotation->y += 5;
		rotation->z += 5;
		break;

	case SHURIKEN:
		currPos = bezier(step, path);

		rotation->y += 5;
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
		p1 = new Point{ targetPos->x, fireHeight, fireHeight / 2 };
		p2 = new Point{ targetPos->x, fireHeight, targetPos->z - fireHeight / 2 };
		break;

	case SHURIKEN:
		// Shuriken
		p1 = new Point{ fireHeight / 2, 0, fireHeight / 2 };
		p2 = new Point{ fireHeight, fireHeight, targetPos->z - fireHeight / 2 };
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
	glBegin(GL_POINTS);
	for (float t = 0; t < 1; t += 0.001)
	{
		Point* p = bezier(t, path);
		glVertex3f(p->x, p->y, p->z);

	}
	glEnd();
	glPopMatrix();
}

bool Weapon:: isCollision(float camAngle, Point* targetObjPos, float targetObjRadius, float roomSize) {

	// Using the camera angle to detect whether it's 
	float distFromDisk = currPos->z * fabs(float(sin(camAngle / 180 * PI)));

	bool targetColl = fabs(currPos->z - targetObjPos ->z) < 1 && distFromDisk < targetObjRadius && currPos->y < targetObjRadius;

	bool wallColl = currPos->z < 0 || currPos->z > roomSize * fabs(float(cos(camAngle / 180 * PI))) + 5;

	return targetColl || wallColl || currPos->y < -targetObjRadius;
}