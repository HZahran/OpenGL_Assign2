#include <iostream>
#include <glut.h>
#include <math.h>
#include <string>
#include "SOIL.h"

using namespace std;


#define PI 3.14159265359f

#define BULLET 0
#define GRENADE 1
#define SHURIKEN 2

#define SPACE_BUTTON 32

//Room
float roomSize = 100;

struct Point {
	double x, y, z;
};

// Target
Point targetPos = *(new Point{ 0, 0, roomSize - 10}); // Dynamic Point Target
Point targetObjPos = targetPos; // Static Obj Target
Point initPos = *(new Point{ 0, 0, 0});
float targetRadius = 5;

//Cam Points
struct Camera {
	Point org; // Eye Point
	Point ref; // Ref Point
	float angle; // Cam Angle
} camera;


struct Triangle {
	Point p1, p2, p3;
};

struct Poly {
	Point p1, p2, p3, p4;
};

struct Path {
	Point p1, p2, p3, p4;
};

struct Weapon {
	int type; // 0 Bullet , 1 Grenade , 2 Shuriken
	bool isFired;
	Point currPoint; // Curr Point
	Path path; // Weapon Bezier
	float scale;
	Point rotation;
	float step; // t of bezier
} weapon;


// Height Of Firing
float fireHeight = 10;


// Replay Mode
bool replayMode;


// Textures
#define TEX_WALL 0
#define TEX_METAL 1

string texPaths[] = {"bricks2.jpg","metal.bmp"};
GLuint texs[sizeof(texPaths)];

// Shapes-------------------------------
void drawCylinder(float pos, float rad1, float rad2,float height,int slices) {
	glPushMatrix();
	glTranslatef(0, 0, pos);
	GLUquadricObj *obj;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_LINE);
	gluCylinder(obj, rad1, rad2, height, slices, 300);
	glPopMatrix();
}

void drawDisk(float pos, float rad1, float rad2) {
	glPushMatrix();
	glTranslatef(0, 0, pos);
	GLUquadricObj *obj;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_LINE);
	gluDisk(obj, rad1, rad2, 300, 300);
	glPopMatrix();
}

void drawSphere(float pos,float rad) {
	glPushMatrix();
	glTranslatef(0, 0, pos);
	glutSolidSphere(rad, 50, 50);
	glPopMatrix();
}

void drawTorus(float pos, float rad1,float rad2) {
	glPushMatrix();
	glTranslatef(0, 0, pos);
	glutSolidTorus(rad1,rad2, 50, 50);
	glPopMatrix();
}

void drawPoly(Poly r , float texRep){
	glPushMatrix();
	glBegin(GL_QUADS);

	glTexCoord3f(texRep * (r.p1.x > 0) , texRep * (r.p1.y > 0) , texRep * (r.p1.z > 0) );
	glVertex3d(r.p1.x, r.p1.y, r.p1.z);
	
	glTexCoord3f(texRep * (r.p2.x > 0) , texRep * (r.p2.y > 0) , texRep * (r.p2.z > 0) );
	glVertex3d(r.p2.x, r.p2.y, r.p2.z);

	glTexCoord3f(texRep * (r.p3.x > 0) , texRep * (r.p3.y > 0) , texRep * (r.p3.z > 0) );
	glVertex3d(r.p3.x, r.p3.y, r.p3.z);

	glTexCoord3f(texRep * (r.p4.x > 0) , texRep * (r.p4.y > 0) , texRep * (r.p4.z > 0) );
	glVertex3d(r.p4.x, r.p4.y, r.p4.z);
	glEnd();
	glPopMatrix();
}

// Bullet ---------------------------------------------------------------------------------

void drawBullet() {
	glPushMatrix();
	
	glColor3d(1, 1, 1);

	// Tail
	drawDisk(0, 0, 2);
	drawTorus(0, 0.3, 2);
	drawTorus(0, 0.3, 1);
	
	int slices = 300;
	// Body
	drawCylinder(0, 2,2,8, slices);
	drawCylinder(8, 2, 1.5, 2 , slices);
	drawCylinder(10, 1.5, 1.5, 3 , slices);
	drawCylinder(13, 1.5, 0.8, 1.5 , slices);
	drawSphere(14.1,0.9);

	glPopMatrix();
}

// Grenade ---------------------------------------------------------------------------------
void drawGrenade() {
	glPushMatrix();
	glRotated(-90, 1, 0, 0);

	// Head
	drawDisk(6, 0, 1.7);
	drawCylinder(4, 1.7, 1.7, 2, 300);
	
	// Body
	drawSphere(0, 5);
	drawTorus(0, 0.3, 5);

	glPopMatrix();
}


// Shuriken ---------------------------------------------------------------------------------
void drawBladeSide(int dir) {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texs[TEX_METAL]);

	double height = 2 * dir;
	double lenTail = 5;
	double lenHead = 15;
	double sidePoint = sqrt(2 * (lenTail / 2)  * (lenTail / 2));

	// Tail
	Poly tailPoly;
	tailPoly.p1 = *(new Point{ 0, 0, 0 });
	tailPoly.p2 = *(new Point{ -sidePoint, 0, sidePoint});
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

void drawBlade(double angle , float pos) {

	glPushMatrix();
	glRotated(angle, 0, 1, 0);
	glTranslatef(0, 0, pos);

	drawBladeSide(1);
	drawBladeSide(-1);
	glPopMatrix();
}

void drawPyramid(float height, bool rotated) {

	glPushMatrix();
	glRotated(180 * rotated, 1, 0, 0);
	drawCylinder(0, height, 0, height, 4);
	glPopMatrix();
}

void drawDiamond(float rad){

	glPushMatrix();
	drawPyramid(rad, 0);
	drawPyramid(rad, 1);
	glPopMatrix();
}

void drawShuriken() {
	glPushMatrix();

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


// Weapon---------------------
void drawWeapon() {
	glPushMatrix();

	glTranslatef(weapon.currPoint.x, weapon.currPoint.y, weapon.currPoint.z);
	glRotated(weapon.rotation.x, 1, 0, 0);
	glRotated(weapon.rotation.y, 0, 1, 0);
	glRotated(weapon.rotation.z, 0, 0, 1);
	glScalef(weapon.scale, weapon.scale, weapon.scale);
	glColor3d(1, 1, 1);

	switch (weapon.type)
	{

	case BULLET:
		drawBullet(); break;
	
	case GRENADE:
		drawGrenade(); break;

	case SHURIKEN:
		drawShuriken(); break;
	default:
		break;
	}

	glPopMatrix();
}

// Room ----------------------------------------

void drawWall(int dir, bool rotX, bool rotY, bool rotZ,bool transX, bool transY, bool transZ) {

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
void drawRoom() {

	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, texs[TEX_WALL]);
	glTranslated(-roomSize/2, -roomSize/2, -roomSize);

	// Front
	drawWall(0, 0, 0, 0, 0, 0, 0);
	
	// Back Wall
	drawWall(2, 0, 1, 0, 1, 0, 1);
	
	// Left Wall
	drawWall(1, 0, 1, 0, 0, 0, 1);

	// Right Wall
	drawWall(-1, 0, 1, 0, 1, 0, 0);

	// Bot Wall
	drawWall(-1, 1, 0, 0, 0, 0, 1);

	// Up Wall
	drawWall(1, 1, 0, 0, 0, 1, 0);

	glPopMatrix();
}

// Target--------------------
void drawTarget() {
	glPushMatrix();
	glRotated(-180, 0, 1, 0);
	glTranslated(targetObjPos.x, targetObjPos.y, targetObjPos.z);

	glNormal3f(0, 0, -1);

	glColor3d(1, 1, 0);
	drawDisk(0, 0, targetRadius);

	glColor3d(1, 0, 0);
	drawDisk(0, 0, targetRadius);

	glColor3d(0, 1, 0);
	drawDisk(0, 0, targetRadius);

	glPopMatrix();
}

void drawScope() {
	glPushMatrix();
	glColor3f(0, 0, 0);

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

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-0.5, 0.5, -0.5, 0.5, -1, 1);
	gluPerspective(90, 640 / 480, 0.001, 500);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camera.org.x, camera.org.y, camera.org.z, camera.ref.x, camera.ref.y, camera.ref.z, 0.0, 1.0, 0.0);

}


Point bezier(float t)
{
	Path path = weapon.path;
	Point res;
	res.x = pow((1 - t), 3)*path.p1.x + 3 * t*pow((1 - t), 2)*path.p2.x + 3 * pow(t, 2)*(1 - t)* path.p3.x + pow(t, 3)* path.p4.x;
	res.y = pow((1 - t), 3)*path.p1.y + 3 * t*pow((1 - t), 2)*path.p2.y + 3 * pow(t, 2)*(1 - t)* path.p3.y + pow(t, 3)* path.p4.y;
	res.z = pow((1 - t), 3)*path.p1.z + 3 * t*pow((1 - t), 2)*path.p2.z + 3 * pow(t, 2)*(1 - t)* path.p3.z + pow(t, 3)* path.p4.z;
	return res;
}

void setPath() {

	Point p1, p2;

	switch (weapon.type)
	{

	case BULLET: return;

	case GRENADE:
		// Grenade
		p1 = *(new Point{ targetPos.x, fireHeight, fireHeight / 2 });
		p2 = *(new Point{ targetPos.x, fireHeight, targetPos.z - fireHeight / 2 });
		break;

	case SHURIKEN:
		// Shuriken
		p1 = *(new Point{ fireHeight / 2, 0, fireHeight / 2 });
		p2 = *(new Point{ fireHeight, fireHeight, targetPos.z - fireHeight / 2 });
		break;
	default:
		break;
	}

	// Path
	weapon.path = *(new Path{ initPos, p1, p2, targetPos });

}

void drawPath() {

	glPushMatrix();

	glPointSize(1);
	glColor3f(1, 1, 1);

	// Grenade & Shuriken
		glBegin(GL_POINTS);
		for (float t = 0; t < 1; t += 0.001)
		{
			Point p = bezier(t);
			glVertex3f(p.x, p.y, p.z);

		}
		glEnd();
	glPopMatrix();
}

void stillWeapon(){
	weapon.currPoint = *(new Point{ 0, -2, 0.5 });
	weapon.scale = 0.2;
	weapon.rotation = *(new Point{ -10, 0, 0 });
	weapon.step = 0;
}

bool isCollision() {

	// Using the camera angle to detect whether it's 
	float distFromDisk =  weapon.currPoint.z * abs(float(sin(camera.angle / 180 * PI)));
	
	bool targetColl = abs(weapon.currPoint.z - targetObjPos.z) < 1 && distFromDisk < targetRadius && weapon.currPoint.y < targetRadius;

	bool wallColl = weapon.currPoint.z < 0 || weapon.currPoint.z > roomSize * abs(float(cos(camera.angle / 180 * PI))) + 5;

	if(targetColl || wallColl)
	std::cout << abs(weapon.currPoint.z - targetObjPos.z) << " "<<targetColl << " "<<wallColl<< std::endl;

	return targetColl || wallColl || weapon.currPoint.y < -targetRadius;
}

void animateWeapon(){

	weapon.currPoint.y = 0;

	switch (weapon.type)
	{
	case BULLET:
		weapon.currPoint.z += 2;

		weapon.rotation.z += 5;
		break;

	case GRENADE:
		weapon.currPoint = bezier(weapon.step);

		weapon.rotation.x += 5;
		weapon.rotation.y += 5;
		weapon.rotation.z += 5;
		break;

	case SHURIKEN:
		weapon.currPoint = bezier(weapon.step);

		weapon.rotation.y += 5;
		break;

	default:
		break;
	}
	
	if (isCollision())
	{

		// End Replay
		if (replayMode)
		{
			weapon.isFired = false;
			replayMode = false;
		}
		// Start Replay
		else {
			//Reset
			stillWeapon();
			replayMode = true;
		}
	}
	else
		weapon.step += 0.01;

}

void anim() {

	// Objects

	// Set Weapon Path
	setPath();

	// Still
	if (!weapon.isFired)
		stillWeapon();

	//Animated
	else animateWeapon();
	
	// Cam
	// Normal 
	if (!replayMode) {
		camera.org = *(new Point{ 0, 0, -4 });
		camera.ref = *(new Point{ 0, 0, 1 });
	}
	// Replay
	else {
		camera.org = *(new Point{ -10, 10, weapon.currPoint.z });;
		camera.ref = weapon.currPoint;
	}


	glutPostRedisplay();
}

void mouse(int x, int y) {
	if (weapon.isFired || x > 400 || x < 240) return;
	camera.angle = x - 320 + 180;
	glutPostRedisplay();
}

void specialKeys(int k, int x, int y)
{
	if (k == GLUT_KEY_RIGHT && targetPos.x > -(roomSize / 2) + 5)
		targetPos.x--;
	if (k == GLUT_KEY_LEFT && targetPos.x < (roomSize / 2) - 5)
		targetPos.x++;
	if (k == GLUT_KEY_UP && targetPos.z < roomSize)
		targetPos.z++;
	if (k == GLUT_KEY_DOWN && targetPos.z > 15)
		targetPos.z--;

	glutPostRedisplay();
}

void key(unsigned char key, int x, int y) {

	if (weapon.isFired) return;

	if (key == SPACE_BUTTON)
		weapon.isFired = true;

	if (key >= '1' && key <= '3')
		weapon.type = key - '0' - 1;

	if (key == 'w' && fireHeight < roomSize/2)
		fireHeight ++;

	if (key == 's' && fireHeight > 10)
		fireHeight --;

	glutPostRedisplay();

}

void Display() {
	setupCamera();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Moving Independant
	glPushMatrix();
	//Rotate Scene
	glColor3f(1, 1, 1);
	glRotatef(camera.angle, 0.0, 1.0, 0.0);
	//glRotated(180, 0, 1, 0);

	glTranslatef(0, 0, 5);

	//Room
	drawRoom();

	//Target
	drawTarget();

	glPopMatrix();


	// Moving with camera
	glPushMatrix();

	// Weapon
	drawWeapon();

	// Path
	if (!weapon.isFired) {
		
		if (weapon.type == BULLET) {
			// Scope
			glPushMatrix();
			glTranslated(0, 0, 0.5);
			glScaled(0.5, 0.5, 0.5);
			drawScope();
			glPopMatrix();
		}
		else {
			//Path Drawing
			drawPath();
		}
	}

	glPopMatrix();
	glFlush();
}

void loadImages() {

	for (int i = 0; i < 2; i++)
	{
		string path = "textures/" + texPaths[i];
		GLuint tex_2d = SOIL_load_OGL_texture
			(
				path.c_str(),
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
				);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		texs[i] = tex_2d;
	}	
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(500, 200);

	glutCreateWindow("Lab 5");
	glutDisplayFunc(Display);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(key);
	glutPassiveMotionFunc(mouse);
	glutIdleFunc(anim);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_BINDING_2D);
	glEnable(GL_TEXTURE_2D);
	
	glShadeModel(GL_SMOOTH);

	camera.angle = 180;
	loadImages();

	glutMainLoop();
}

/*float camAngleRad = (camAngle / 180 * PI);
float changeX = float(sin(camAngleRad)) * movSpeed;
float changeZ = float(cos(camAngleRad)) * movSpeed;

float newX = xpos;
float newZ = zpos;

if (key == 'w')
{
newX += changeX;
newZ -= changeZ;
}

if (key == 's')
{
newX -= changeX;
newZ += changeZ;
}

if (key == 'd')
{
newX += changeZ;
newZ += changeX;
}

if (key == 'a')
{
newX -= changeZ;
newZ -= changeX;
}*/
//std::cout << newX << " " << newZ << endl;

/*if(newX > -(roomSize/2) && newX < roomSize/2)
xpos = newX;

if(newZ > 0 && newZ < roomSize)
zpos = newZ;*/

//void axis(double length) { // draw a z-axis, with cone at end  
//	glPushMatrix();
//
//	glBegin(GL_LINES);
//	glVertex3d(0, 0, 0);
//	glVertex3d(0, 0, length); // along the z-axis 
//	glEnd();
//
//	glTranslated(0, 0, length - 0.2);
//	glutWireCone(0.04, 0.2, 12, 9);
//
//	glPopMatrix();
//}
//
//void drawAxis() {
//	glPushMatrix();
//	glColor3d(0, 0, 0); // draw black lines       
//	axis(10);      // z-axis 
//
//	glPushMatrix();
//	glRotated(90, 0, 1.0, 0);
//	axis(10);     // y-axis
//	glRotated(-90.0, 1, 0, 0);
//	axis(10);     // x-axis
//	glPopMatrix();
//
//	glPopMatrix();
//}