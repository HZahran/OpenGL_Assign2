#include "Target.h"

void Target:: draw() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texID);
	glTranslated(currPos->x, currPos->y, currPos->z);

	glNormal3f(0, 0, -1);

	//GLfloat color[] = YELLOW;
	//glColor3fv(color);

	drawDisk(0, 0, radius);


	glPopMatrix();
}