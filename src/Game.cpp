#include <iostream>
#include <irrKlang\irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

#include <string>
#include "Game.h"
#include "SOIL.h"

using namespace std;

// Textures
#define TEX_WALL 0
#define TEX_METAL 1
#define TEX_GRENADE 2
#define TEX_TARGET 3

#define NUM_TEXS 4

string texPaths[] = {"bricks2.jpg","metal.bmp","grenade.jpg","target.png"};
GLuint texs[NUM_TEXS];

bool isCollision() {

	float distFromDiskX = fabs(target->currPos->x + camera->angle);
	float distFromDiskZ = sqrt(weapon->currPos->z * weapon->currPos->z - target->currPos->x * target->currPos->x);

		//fabs( target->currPos->x + (weapon->currPos->z * float(sin( camera->angle / 180 * PI) )));

	bool targetColl = fabs(distFromDiskZ - target->currPos->z) < 1 && distFromDiskX < target->radius && weapon->currPos->y < target->radius;

	bool wallColl = weapon->currPos->z < 0 || weapon->currPos->z > room->size * fabs(float(cos(camera->angle / 180 * PI)));
	
	if (targetColl) {
		//PlaySound(TEXT("sounds/maysa7esh.wav"), NULL, SND_ASYNC | SND_FILENAME);
	}
	
	return targetColl || wallColl || weapon->currPos->y < -target->radius;
}

void handleWeapon() {

	// Set Weapon Path
	weapon ->animate();

	if (isCollision())
	{

		// End Replay
		if (replayMode)
		{
			weapon ->isFired = false;
			replayMode = false;
		}
		// Start Replay
		else {
			//Reset
			weapon ->still();
			replayMode = true;
		}
	}
	else
		weapon ->step += (0.002 - 0.0005 * replayMode); // Slow Down during replay
}

void handleCamera() {
	// Normal 
	if (!replayMode)
		camera ->reset();
	// Replay
	else {
		// Track Weapon
		camera ->org = new Point{ -10, 10, weapon ->currPos->z };
		camera ->ref = weapon ->currPos;
	}
}

void anim() {

	if (glutGet(GLUT_ELAPSED_TIME) < FADE_IN_TIME) ambient += 0.002;

	handleWeapon();
	
	handleCamera();

	glutPostRedisplay();
}

void mouse(int x, int y){
	if (weapon ->isFired || x > 400 || x < 240) return;
	camera ->angle = x - 320;
	glutPostRedisplay();
}

void specialKeys(int k, int x, int y)
{
	if (k == GLUT_KEY_RIGHT && weapon ->targetPos->x > -(room ->size / 2) + 5)
		weapon ->targetPos->x--;
	if (k == GLUT_KEY_LEFT && weapon ->targetPos->x < (room ->size / 2) - 5)
		weapon ->targetPos->x++;
	if (k == GLUT_KEY_UP && weapon ->targetPos->z < room ->size)
		weapon ->targetPos->z++;
	if (k == GLUT_KEY_DOWN && weapon ->targetPos->z > 15)
		weapon ->targetPos->z--;

	glutPostRedisplay();
}

void keys(unsigned char key, int x, int y) {

	if (weapon ->isFired) return;

	if (key == SPACE_BUTTON)
		weapon ->isFired = true;

	if (key >= '1' && key <= '3')
		weapon ->type = key - '0' - 1;

	if (key == 'w' && weapon ->fireHeight < room ->size/2)
		weapon ->fireHeight ++;

	if (key == 's' && weapon ->fireHeight > 5)
		weapon ->fireHeight --;

	glutPostRedisplay();

}

// Surrounding Scene
void drawScene() {
	glPushMatrix();

	//Rotate Scene
	glRotatef(camera ->angle, 0.0, 1.0, 0.0);

	//glTranslatef(0, 0, -5);

	//Room
	room ->draw();

	//Target
	target ->draw();

	glPopMatrix();
}

// FPS View
void drawView() {
	glPushMatrix();

	GLfloat shininess[] = { 150 };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat color[] = WHITE;
	glColor3fv(color);

	// Weapon

	if (weapon->type == SHURIKEN) weapon->texID = texs[TEX_METAL];
	else weapon->texID = texs[TEX_GRENADE];

	weapon ->draw();

	// Path
	if (!weapon ->isFired) {

		if (weapon ->type == BULLET)
			drawScope();
		else
			weapon ->drawPath();
	}

	glPopMatrix();
}

void setupLight() {
	
	GLfloat l0Diffuse[] = BROWN;
	GLfloat l0Ambient[] = { ambient - 0.2, ambient - 0.1 ,ambient, 1.0f };
	GLfloat l0Position[] = { 0, 0, -5 , 1};
	GLfloat l0Direction[] = { 0.0f, 0.0f, room->size};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0Diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0Ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, l0Position);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 70 - 30*fabs(float(cos(camera->angle / 180 * PI))));
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);
}

void display() {

	camera ->setup();
	setupLight();


	GLfloat color[] = WHITE;
	glColor3fv(color);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Moving Independant
	drawScene();

	// Moving with Camera
	drawView();

	glFlush();
}

void loadImages() {

	for (int i = 0; i < NUM_TEXS; i++)
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

void playSound(string filename) {
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
	if (!engine) return; // could not start engine		
	
	string path = "sounds/" + filename;
	engine->play2D(path.c_str(), true); // play some mp3 file, looped
						
}

void init() {

	// Room
	room = new Room{100 , texs[TEX_WALL]};

	// Target

	target = new Target{ new Point{ -10, 0, room->size - 50 } , // Curr Point
		 5,  // Radius
		 texs[TEX_TARGET]
	};

	// Weapon
	weapon = new Weapon{};
	
	weapon->texID = texs[TEX_GRENADE];
	weapon->still();
	weapon ->targetPos =  new Point{ 0, 0, room->size - 10 };
	weapon ->initPos = new Point{ 0, 0, 6};
	weapon ->fireHeight = 5;

	// Camera
	camera = new Camera{};
	camera ->reset();
}


void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(500, 200);

	glutCreateWindow("FPS GAME");
	glutDisplayFunc(display);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keys);
	glutPassiveMotionFunc(mouse);
	glutIdleFunc(anim);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_SHININESS);
	glEnable(GL_COLOR_MATERIAL_FACE);

	glShadeModel(GL_SMOOTH);

	// Background music
	playSound("main-theme.wav");

	loadImages();
	init();

	glutMainLoop();
}