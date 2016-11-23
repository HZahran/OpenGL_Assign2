#include "Shapes.h"

void drawCylinder(float pos, float rad1, float rad2, float height, int slices) {
	glPushMatrix();
	glTranslatef(0, 0, pos);
	GLUquadricObj *obj;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_LINE);
	gluQuadricTexture(obj, true);
	gluQuadricNormals(obj, GLU_SMOOTH);
	gluCylinder(obj, rad1, rad2, height, slices, 100);
	gluDeleteQuadric(obj);
	glPopMatrix();
}

void drawDisk(float pos, float rad1, float rad2) {
	glPushMatrix();
	glTranslatef(0, 0, pos);
	GLUquadricObj *obj;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_LINE);
	gluQuadricTexture(obj, true);
	gluQuadricNormals(obj, GLU_SMOOTH);
	gluDisk(obj, rad1, rad2, 100, 100);
	gluDeleteQuadric(obj);
	glPopMatrix();
}

void drawSphere(float pos, float rad) {
	glPushMatrix();
	glTranslatef(0, 0, pos);
	GLUquadricObj* sphere = gluNewQuadric();
	gluQuadricTexture(sphere, true);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, rad, 50, 50);
	gluDeleteQuadric(sphere);
	glPopMatrix();
}

void drawTorus(float pos, float rad1, float rad2) {
	glPushMatrix();
	glTranslatef(0, 0, pos);
	glutSolidTorus(rad1, rad2, 50, 50);
	glPopMatrix();
}

void drawPoly(Poly r, float texRep) {
	glPushMatrix();
	glBegin(GL_POLYGON);

	glTexCoord3f(texRep * (r.p1.x > 0), texRep * (r.p1.y > 0), texRep * (r.p1.z > 0));
	glVertex3d(r.p1.x, r.p1.y, r.p1.z);

	glTexCoord3f(texRep * (r.p2.x > 0), texRep * (r.p2.y > 0), texRep * (r.p2.z > 0));
	glVertex3d(r.p2.x, r.p2.y, r.p2.z);

	glTexCoord3f(texRep * (r.p3.x > 0), texRep * (r.p3.y > 0), texRep * (r.p3.z > 0));
	glVertex3d(r.p3.x, r.p3.y, r.p3.z);

	glTexCoord3f(texRep * (r.p4.x > 0), texRep * (r.p4.y > 0), texRep * (r.p4.z > 0));
	glVertex3d(r.p4.x, r.p4.y, r.p4.z);


	glEnd();
	glPopMatrix();
}

void drawPyramid(float height, bool rotated) {

	glPushMatrix();
	glRotated(180 * rotated, 1, 0, 0);
	drawCylinder(0, height, 0, height, 4);
	glPopMatrix();
}

void drawDiamond(float rad) {

	glPushMatrix();
	drawPyramid(rad, 0);
	drawPyramid(rad, 1);
	glPopMatrix();
}

void drawWall(float roomSize, int dir, bool rotX, bool rotY, bool rotZ, bool transX, bool transY, bool transZ) {

	glPushMatrix();

	glTranslated(roomSize * transX, roomSize * transY, roomSize * transZ);
	glRotated(90 * dir, rotX, rotY, rotZ);

	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(0, 0);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(0, roomSize);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(roomSize, roomSize);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(roomSize, 0);

	glEnd();
	glPopMatrix();
}

void drawScope() {
	glPushMatrix();
	glTranslated(0, 0, 9.5);
	glScaled(0.5, 0.5, 0.5);

	glNormal3d(0, 0, -1);

	glBegin(GL_LINES);

	// Right
	glVertex3d(0.2, 0, 0);
	glVertex3d(0.7, 0, 0);

	// Left
	glVertex3d(-0.2, 0, 0);
	glVertex3d(-0.7, 0, 0);

	// Up
	glVertex3d(0, 0.2, 0);
	glVertex3d(0, 0.7, 0);

	//Down
	glVertex3d(0, -0.2, 0);
	glVertex3d(0, -0.7, 0);

	glEnd();
	glPopMatrix();
}

Point* bezier(float t , Path* path)
{
	Point *p1 = path->p1;
	Point *p2 = path->p2;
	Point *p3 = path->p3;
	Point *p4 = path->p4;

	Point *res = new Point();
	res->x = pow((1 - t), 3)* p1->x + 3 * t*pow((1 - t), 2)* p2->x + 3 * pow(t, 2)*(1 - t)* p3->x + pow(t, 3)* p4->x;
	res->y = pow((1 - t), 3)* p1->y + 3 * t*pow((1 - t), 2)* p2->y + 3 * pow(t, 2)*(1 - t)* p3->y + pow(t, 3)* p4->y;
	res->z = pow((1 - t), 3)* p1->z + 3 * t*pow((1 - t), 2)* p2->z + 3 * pow(t, 2)*(1 - t)* p3->z + pow(t, 3)* p4->z;
	return res;
}
