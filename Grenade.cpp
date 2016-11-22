#include "Grenade.h"

void drawGrenade() {
	glPushMatrix();
	glRotated(-90, 1, 1, 0);
	// Head
	drawDisk(6, 0, 1.7);
	drawCylinder(4, 1.7, 1.7, 2, 300);

	// Body
	drawSphere(0, 5);
	drawTorus(0, 0.3, 5);

	glPopMatrix();
}