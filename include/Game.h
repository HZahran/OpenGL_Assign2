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

// Score
int score = 0;

// Replay Mode
bool replayMode;
float replayAngle = -1; // Cam angle of replay
Point targetReplayPos; // Target Pos at replay
Point targetCurrPos; // Temp for returning after replay
int replayWeapon;

// Target Fire
bool isHit;
float fireTimer;

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
