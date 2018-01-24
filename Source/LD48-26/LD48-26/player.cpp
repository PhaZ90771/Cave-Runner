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

Player::Player(float x, float y, int screenWidth, int screenHeight) {
	mW = 50;
	mH = 50;
	mX = x - mW / 2;
	mY = y - mH / 2;
	mDX = 0;
	mDX = 0;
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	mMoveSpeed = 27.f / 100;
	mHP = 10;

	mHit = true;
	mNextHitTime = SDL_GetTicks();
	mHitDelay = 500;

	mFlashDelay = 100;
	mFlashColor = true;

	mInput.left = false;
	mInput.right = false;
	mInput.up = false;
	mInput.down = false;
}

SDL_Rect Player::GetPosition() {
	SDL_Rect rect;

	rect.x = (Sint16)mX;
	rect.y = (Sint16)mY;
	rect.w = (Sint16)mW;
	rect.h = (Sint16)mH;
	
	return rect;
}

void Player::SetLeft(bool left) {
	mInput.left = left;
}

void Player::SetRight(bool right) {
	mInput.right = right;
}

void Player::SetUp(bool up) {
	mInput.up = up;
}

void Player::SetDown(bool down) {
	mInput.down = down;
}

void Player::Move() {
	mX += mDX;
	mY += mDY;
}

void Player::UpdateSpeed() {
	mDX = 0;
	mDY = 0;

	if(mInput.left)
		mDX -= 1;
	if(mInput.right)
		mDX += 1;
	if(mInput.up)
		mDY += 1;
	if(mInput.down)
		mDY -= 1;

	mDX = mDX * mMoveSpeed;
	mDY = mDY * mMoveSpeed;
}

void Player::Bump(float x) {
	mX += x;
}

void Player::Hit() {
	if(SDL_GetTicks() >= mNextHitTime) {
		mHit = true;
		mFlashColor = true;
		mHP--;
		mNextHitTime = SDL_GetTicks() + mHitDelay;
		mNextFlashTime = SDL_GetTicks() + mFlashDelay;
	}
}

bool Player::Update() {
	if(mHP <= 0)
		return true;

	if( (mHit) && (SDL_GetTicks() >= mNextHitTime) ) {
		mHit = false;
		mFlashColor = false;
	}

	if( (mHit) && (SDL_GetTicks() >= mNextFlashTime) ) {
		if(mFlashColor) {
			mFlashColor = false;
			mNextFlashTime = SDL_GetTicks() + mFlashDelay;
		}
		else {
			mFlashColor = true;
			mNextFlashTime = SDL_GetTicks() + mFlashDelay;
		}
	}

	UpdateSpeed();

	Move();

	if(mX < -(float)mScreenWidth)
		mX = -(float)mScreenWidth;
	if(mX+mW > (float)mScreenWidth)
		mX = (float)mScreenWidth - mW;
	if(mY < 0)
		mY = 0;
	if(mY+mH > (float)mScreenHeight*2)
		mY = ((float)mScreenHeight*2) - mH;

	return false;
}

void Player::Render() {

	// Player GUI

	if(mFlashColor) {
		glColor3f( 1.f, 27.f / 100.f, 0.f );
	}
	else {
		glColor3f( 1.f, 1.f, 1.f );
	}

	glBegin(GL_QUAD_STRIP);
		glVertex2f( mX   , mY);
		glVertex2f( mX+mH, mY);
		glVertex2f( mX   , mY+mH);
		glVertex2f( mX+mH, mY+mH);
	glEnd();
}