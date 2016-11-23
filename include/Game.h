#include "Shapes.h"
#include "Weapon.h"
#include "Room.h"
#include "Target.h"
#include "Camera.h"


#define SPACE_BUTTON 32
#define FADE_IN_TIME 3000

// Game Objects
Room* room;
Target * target;
Weapon * weapon;
Camera * camera;

// Light 
float ambient = 0;


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
