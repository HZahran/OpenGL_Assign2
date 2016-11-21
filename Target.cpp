#include "Target.h"

void Target:: draw() {
	glPushMatrix();
	glRotated(-180, 0, 1, 0);
	glTranslated(currPos->x, currPos->y, currPos->z);

	glNormal3f(0, 0, -1);

	glColor3d(1, 1, 0);
	drawDisk(0, 0, radius);

	glColor3d(1, 0, 0);
	drawDisk(0, 0, radius);

	glColor3d(0, 1, 0);
	drawDisk(0, 0, radius);

	glPopMatrix();
}