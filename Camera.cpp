#include "Camera.h"

void Camera::reset() {
	org = new Point{ 0, 0, -4 };
	ref = new Point{ 0, 0, 1 };
}

void Camera:: setup() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 640 / 480, 0.001, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(org->x, org->y, org->z, ref->x, ref->y, ref->z, 0.0, 1.0, 0.0);
}