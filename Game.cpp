#include <iostream>
#include <string>
#include "Game.h"
#include "SOIL.h"

using namespace std;

string texPaths[] = {"bricks2.jpg","metal.bmp"};
GLuint texs[NUM_TEXS];

void handleWeapon() {
	// Set Weapon Path
	weapon ->animate();

	if (weapon->isCollision(camera->angle , target->currPos, target->radius, room->size))
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
		weapon ->step += 0.01;
}

void handleCamera() {
	// Normal 
	if (!replayMode)
		camera ->reset();
	// Replay
	else {
		camera ->org = new Point{ -10, 10, weapon ->currPos->z };
		camera ->ref = weapon ->currPos;
	}
}

void anim() {

	handleWeapon();
	
	handleCamera();

	glutPostRedisplay();
}

void mouse(int x, int y) {
	if (weapon ->isFired || x > 400 || x < 240) return;
	camera ->angle = x - 320 + 180;
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

	if (key == 's' && weapon ->fireHeight > 10)
		weapon ->fireHeight --;

	glutPostRedisplay();

}


// Surrounding Scene
void drawScene() {
	glPushMatrix();

	//Rotate Scene
	glColor3f(1, 1, 1);
	glRotatef(camera ->angle, 0.0, 1.0, 0.0);

	glTranslatef(0, 0, 5);

	//Room
	room ->draw();

	//Target
	target ->draw();

	glPopMatrix();
}

// FPS View
void drawView() {
	glPushMatrix();

	// Weapon
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


void display() {

	camera ->setup();

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

	target = new Target{ new Point{ 0, 0, room ->size - 10 } , // Curr Point
		 5 }; // Radius

	// Weapon
	weapon = new Weapon{};
	
	weapon->texID = texs[TEX_METAL];
	weapon->still();
	weapon ->targetPos = target ->currPos;
	weapon ->initPos = new Point{ 0, 0, 0 };
	weapon ->fireHeight = 10;

	// Camera
	camera = new Camera{};
	camera ->reset();
	camera ->angle = 180;

}

void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(500, 200);

	glutCreateWindow("Lab 5");
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
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_BINDING_2D);
	glEnable(GL_TEXTURE_2D);
	
	glShadeModel(GL_SMOOTH);

	loadImages();
	init();

	glutMainLoop();
}