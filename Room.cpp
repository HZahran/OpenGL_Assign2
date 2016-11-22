#include "Room.h"

void Room:: draw() {

	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, texID);
	glTranslated(-size / 2, -size / 2, 0);

	// Front
	drawWall(size, 0, 0, 0, 0, 0, 0, 0);

	// Back Wall
	drawWall(size, 2, 0, 1, 0, 1, 0, 1);

	// Left Wall
	drawWall(size, 1, 0, 1, 0, 0, 0, 1);

	// Right Wall
	drawWall(size, -1, 0, 1, 0, 1, 0, 0);

	// Bot Wall
	drawWall(size, -1, 1, 0, 0, 0, 0, 1);

	// Up Wall
	drawWall(size, 1, 1, 0, 0, 0, 1, 0);

	glPopMatrix();
}