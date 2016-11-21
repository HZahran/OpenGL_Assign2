#include "Shapes.h"
#include "Weapon.h"
#include "Room.h"
#include "Target.h"
#include "Camera.h"


#define SPACE_BUTTON 32

// Textures
#define NUM_TEXS 2

#define TEX_WALL 0
#define TEX_METAL 1


// Game Objects
Room* room;
Target * target;
Weapon * weapon;
Camera * camera;

// Replay Mode
bool replayMode;

void handleWeapon();

void handleCamera();

void anim();

void mouse(int x, int y);

void specialKeys(int k, int x, int y);

void keys(unsigned char key, int x, int y);

void drawScene();

void drawView();

void setupLight();

void display();

void loadImages();

void init();
