#include "Shuriken.h"

void drawBladeSide(int dir) {
	glPushMatrix();

	double height = 2 * dir;
	double lenTail = 5;
	double lenHead = 15;
	double sidePoint = sqrt(2 * (lenTail / 2)  * (lenTail / 2));

	// Tail
	Poly tailPoly;
	tailPoly.p1 = *(new Point{ 0, 0, 0 });
	tailPoly.p2 = *(new Point{ -sidePoint, 0, sidePoint });
	tailPoly.p3 = *(new Point{ 0, height, lenTail });
	tailPoly.p4 = *(new Point{ sidePoint, 0, sidePoint });
	drawPoly(tailPoly, 0.75);

	// Body
	Poly headPoly;
	headPoly.p1 = *(new Point{ 0, 0, lenHead });
	headPoly.p2 = tailPoly.p2;
	headPoly.p3 = tailPoly.p3;
	headPoly.p4 = tailPoly.p4;
	drawPoly(headPoly, 0.25);

	glPopMatrix();
}

void drawBlade(double angle, float pos) {

	glPushMatrix();

	glRotated(angle, 0, 1, 0);
	glTranslatef(0, 0, pos);

	drawBladeSide(1);
	drawBladeSide(-1);
	glPopMatrix();
}

void drawShuriken(GLuint texID) {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texID);

	float diamondRad = 3;
	// Body
	drawDiamond(diamondRad);

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	drawTorus(0, 0.5, diamondRad);
	glPopMatrix();

	// Blades
	drawBlade(0, diamondRad);
	drawBlade(120, diamondRad);
	drawBlade(240, diamondRad);

	glPopMatrix();
}