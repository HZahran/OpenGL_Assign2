#include <iostream>
#include <irrKlang\irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

#include <string>
#include "Game.h"
#include "SOIL.h"

using namespace std;


// OnScreen Alerts
#define ALERT_TIME 5000
#define NUM_ALERTS 4
string onScreenText[NUM_ALERTS] = { "Use Space To Fire", "Use Arrows To Move Target" , "Use W-A-S-D For Aiming","Use R To Replay"};

// Textures
#define TEX_WALL 0
#define TEX_METAL 1
#define TEX_GRENADE 2
#define TEX_TARGET 3
#define TEX_TARGET_FIRE 4

#define NUM_TEXS 5

string texPaths[] = {"bricks2.jpg","metal.bmp","grenade.jpg","target.png","fire.jpg"};
GLuint texs[NUM_TEXS];

//Sound
ISoundEngine* soundEngine = createIrrKlangDevice();
string sounds[] = { "gun.mp3","grenade.mp3","shuriken.wav" };

void drawText(string text, float x, float y) {
    
	glPushMatrix();
	
	glNormal3d(0, 0, -1);
	glColor3d(1, 1, 1);
	glRasterPos3f(x, y, 6);
	for (unsigned int i = 0; i< text.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);

	glPopMatrix();
	
}

void playSound(string filename, bool loop) {
	string path = "sounds/" + filename;
	soundEngine->play2D(path.c_str(), loop);
}

bool isCollision() {

	float distFromDiskX = fabs(target->currPos->x + (weapon->currPos->z * float(sin(camera->angle / 180 * PI))));
	//float distFromDiskZ = sqrt(weapon->currPos->z * weapon->currPos->z - target->currPos->x * target->currPos->x);


	bool targetColl = fabs(weapon->currPos->z - target->currPos->z) < 1 && distFromDiskX < target->radius && weapon->currPos->y < target->radius;

	bool wallColl = weapon->currPos->z < 0 || weapon->currPos->z > room->size * fabs(float(cos(camera->angle / 180 * PI)));

	if (targetColl) {
		score += 5;
		target->texID = texs[TEX_TARGET_FIRE];
		isHit = true;
		if(!replayMode)
		playSound("hit.mp3", false);
	}
	
	return targetColl || wallColl || weapon->currPos->y < -target->radius;
}

void handleWeapon() {

	// Set Weapon Path
	weapon ->animate();

	if (isCollision())
	{

		if (replayMode) {
			replayMode = false;
			target->currPos = &(targetCurrPos); // Reset To current pos
		}
		else
			weapon ->still();
		
		weapon->isFired = false;
	}
	else
		weapon ->step += (0.002 - 0.001 * replayMode); // Slow Down during replay
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
	
	// Fade Animation
	if (glutGet(GLUT_ELAPSED_TIME) < FADE_IN_TIME) ambient += 0.004;
	else ambient = 0.5;

	// Target Fire Anim
	if (weapon->type == GRENADE && isHit && !replayMode && fireTimer < 5000)
		fireTimer += 10;
	else {
		isHit = false;
		fireTimer = 0;
		target->texID = texs[TEX_TARGET];
	}

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
	if (k == GLUT_KEY_RIGHT && target->currPos->x > -(room ->size / 2) + target->radius)
		target->currPos->x--;
	if (k == GLUT_KEY_LEFT && target->currPos->x < (room ->size / 2) - target->radius)
		target->currPos->x++;
	if (k == GLUT_KEY_UP && target->currPos->z < room ->size)
		target->currPos->z++;
	if (k == GLUT_KEY_DOWN && target->currPos->z > room -> size/2)
		target->currPos->z--;

	glutPostRedisplay();
}

void keys(unsigned char key, int x, int y) {

	if (weapon ->isFired) return;

	if (key == SPACE_BUTTON) {

		// Define curr state for replay
		replayAngle = camera->angle;
		targetReplayPos = *(target->currPos);
		replayWeapon = weapon->type;

		weapon->isFired = true;
		playSound(sounds[weapon->type], false);
	}

	if (key >= '1' && key <= '3') {
		weapon->type = key - '0' - 1;

		// Reset Path
		weapon->targetPos = new Point{ 0, 0, room->size - 10 };
	}

	if (key == 'w' && weapon ->fireHeight < room ->size/2)
		weapon ->fireHeight ++;

	if (key == 's' && weapon ->fireHeight > 1)
		weapon ->fireHeight --;

	if (key == 'd' && target->currPos->x > -(room->size / 2) + target->radius)
		weapon->targetPos->x--;

	if (key == 'a' && target->currPos->x < (room->size / 2) - target->radius)
		weapon->targetPos->x++;

	if (key == 'r' && replayAngle !=-1 && !replayMode)
	{
		// Retrieve state
		camera->angle = replayAngle;
		targetCurrPos = *(target->currPos);
		target->currPos = &(targetReplayPos);
		weapon->type = replayWeapon;

		replayMode = true;
		weapon->isFired = true;
	}
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


void drawOnScreenText() {

	// Alert
	if (glutGet(GLUT_ELAPSED_TIME) < NUM_ALERTS * ALERT_TIME)
		drawText(onScreenText[int(glutGet(GLUT_ELAPSED_TIME) / ALERT_TIME)], 0.5, 0.8);

	// Score
	string scoreText = "Score : "+ to_string(score);
	drawText(scoreText, -0.5, 0.8);
}

// FPS Screen
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

	// Path && Text
	if (!weapon ->isFired) {

		glBindTexture(GL_TEXTURE_2D, NULL);
		
		drawOnScreenText();

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


void init() {

	// Room
	room = new Room{100 , texs[TEX_WALL]};

	// Target
	target = new Target{ new Point{ 10, 0, room->size - 50 } , // Curr Point
		 5,  // Radius
		 texs[TEX_TARGET]
	};

	// Weapon
	weapon = new Weapon{};
	
	weapon->texID = texs[TEX_GRENADE];
	weapon->still();
	weapon ->targetPos =  new Point{ 0, 0, room->size - 10 };
	weapon ->initPos = new Point{ 0, 0, 9};
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
	playSound("main-theme.wav",true);

	loadImages();
	init();

	glutMainLoop();
}