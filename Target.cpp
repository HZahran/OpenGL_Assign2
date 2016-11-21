#include "Target.h"

void Target:: draw() {
	glPushMatrix();
	glRotated(-180, 0, 1, 0);
	glTranslated(currPos->x, currPos->y, currPos->z);

	glNormal3f(0, 0, -1);

	GLfloat color[] = YELLOW;
	glColor3fv(color);

	drawDisk(0, 0, radius);


	glPopMatrix();
}