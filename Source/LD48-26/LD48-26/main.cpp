#pragma region define
#define _USE_MATH_DEFINES
#pragma endregion
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_opengl.h"
#include "freeglut.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include "utilities.h"
#include "level.h"
#include "engine.h"

int main(int argc, char** argv) {
	Engine gameEngine = Engine(800, 600, 32);

	gameEngine.Run();
	
	return 0;
}