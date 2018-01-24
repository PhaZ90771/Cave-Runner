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
#include "player.h"

Engine::Engine(int width, int height, int bpp) {
	srand((unsigned int)time(NULL));

	// Set Window Attributes
	mScreenWidth = width;
	mScreenHeight = height;
	mScreenBPP = bpp;
	mFPS = 60;

	// Window Caption
	mCaption = "Cave Runner -- Ludum Dare 26: Minimalism [PhaZ90771]";

	// Set State
	mQuit = false;
	
	// Initialize Window
	Init();

	// Initialize OpenGL
	InitOpenGL();

	GenerateLevel();
	CreatePlayer();
}

Engine::~Engine() {
	delete mLevel;
	delete mPlayer;

	// Quit SDL
	SDL_Quit();
}

void Engine::Init() {
	// Init SDL
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		exit(1);
	}

	// Set up OpenGL attribures
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Setup Screen
	if((mScreen = SDL_SetVideoMode(mScreenWidth, mScreenHeight, mScreenBPP, SDL_OPENGL)) == NULL) {
		exit(2);
	}

	// Set Window Caption
	SDL_WM_SetCaption(mCaption.c_str(), NULL);

	InitOpenGL();
}

void Engine::InitOpenGL() {
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	glViewport((GLint)0.f, (GLint)0.f, mScreenWidth, mScreenHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, mScreenWidth * 2, 0, mScreenHeight * 2);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor((float)0.6, (float)0.6, (float)0.6, 1.f);

	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		exit(error);
	}
}

void Engine::GenerateLevel() {
	mLevel = new Level(40, 0.f, 0.f, 20, 60, 400.f, 180, 300, mScreenWidth, mScreenHeight, 0.40f);
}

void Engine::CreatePlayer() {
	mPlayer = new Player(0.f, (float)mScreenHeight, mScreenWidth, mScreenHeight);

	CollisionType c;
	while((c = mLevel->CheckCollision(mPlayer->GetPosition())) != NOCOLLISION) {
		if(c == LEFTWALL)
			mPlayer->Bump(1.f);
		else if(c == RIGHTWALL)
			mPlayer->Bump(-1.f);
	}
}

void Engine::Update() {
	if(SDL_PollEvent(&mEvent)) {
		if((mEvent.type == SDL_QUIT) || ((mEvent.type == SDL_KEYDOWN) && (mEvent.key.keysym.sym == SDLK_ESCAPE))) {
			mQuit = true;
		}
		else if(mEvent.type == SDL_KEYDOWN) {
			if(mEvent.key.keysym.sym == SDLK_LEFT)
				mPlayer->SetLeft(true);
			if(mEvent.key.keysym.sym == SDLK_RIGHT)
				mPlayer->SetRight(true);
			if(mEvent.key.keysym.sym == SDLK_UP)
				mPlayer->SetUp(true);
			if(mEvent.key.keysym.sym == SDLK_DOWN)
				mPlayer->SetDown(true);
		}
		else if(mEvent.type == SDL_KEYUP) {
			if(mEvent.key.keysym.sym == SDLK_LEFT)
				mPlayer->SetLeft(false);
			if(mEvent.key.keysym.sym == SDLK_RIGHT)
				mPlayer->SetRight(false);
			if(mEvent.key.keysym.sym == SDLK_UP)
				mPlayer->SetUp(false);
			if(mEvent.key.keysym.sym == SDLK_DOWN)
				mPlayer->SetDown(false);
		}
	}

	mLevel->Update();
	if(mPlayer->Update())
		mQuit = true;

	CollisionType c;
	bool d = false;
	while((c = mLevel->CheckCollision(mPlayer->GetPosition())) != NOCOLLISION) {
		d = true;
		if(c == LEFTWALL)
			mPlayer->Bump(1.f);
		else if(c == RIGHTWALL)
			mPlayer->Bump(-1.f);
	}

	if(d)
		mPlayer->Hit();
}

void Engine::RenderGUI() {
	// Health GUI
	glColor3f( 1.f, 0.f, 0.f );

	int offset = 10;
	int w = 20;
	int h = 30;
	int x = (-mScreenWidth) + (2 * offset);
	int y = (2 * mScreenHeight) - (2 * offset) - h;

	for(int i = 0; i < mPlayer->GetHP(); i++) {
		glBegin(GL_QUADS);
			glVertex2f( (GLfloat)x  , (GLfloat)y);
			glVertex2f( (GLfloat)x+w, (GLfloat)y);
			glVertex2f( (GLfloat)x+w, (GLfloat)y+h);
			glVertex2f( (GLfloat)x  , (GLfloat)y+h);
		glEnd();

		x += w + offset;
	}
}

void Engine::Render() {
	// Setup
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef( (GLfloat)mScreenWidth, (GLint)0.f, (GLint)-1.f );

	mPlayer->Render();
	mLevel->Render();
	RenderGUI();

	// Swap Screen
	SDL_GL_SwapBuffers();
}

void Engine::FPS() {
	if(mFPSTimer.getTicks() < 1000 / mFPS) {
		SDL_Delay((1000 / mFPS) - mFPSTimer.getTicks());
	}
}

void Engine::Run() {
	mFPSTimer.start();

	while(!mQuit) {
		Update();
		Render();
		FPS();
	}
}