#include "Bullet.h"

void drawBullet() {
	glPushMatrix();

	// Tail
	drawDisk(0, 0, 2);
	drawTorus(0, 0.3, 2);
	drawTorus(0, 0.3, 1);

	int slices = 300;
	// Body
	drawCylinder(0, 2, 2, 8, slices);
	drawCylinder(8, 2, 1.5, 2, slices);
	drawCylinder(10, 1.5, 1.5, 3, slices);
	drawCylinder(13, 1.5, 0.8, 1.5, slices);
	drawSphere(14.1, 0.9);

	glPopMatrix();
}