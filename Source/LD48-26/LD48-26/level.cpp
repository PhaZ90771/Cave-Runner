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

// Line Node
#pragma region LineNode
LineNode::LineNode(float x, float y, float x2, float y2, LineNode* before, LineNode* after) {
	 line.x = x;
	 line.y = y;
	 line.x2 = x2;
	 line.y2 = y2;
	 prev = before;
	 next = after;
}

float LineNode::GetWidth() {
	return line.x2-line.x;
}

float LineNode::GetMiddleX() {
	return ((line.x + line.x2)/2);
}

float LineNode::GetMiddleY() {
	return ((line.y + line.y2)/2);
}
#pragma endregion

// Line Node List
#pragma region LineNodeList
LineNodeList::LineNodeList(LineNode* lineNode) {
	start = lineNode;
	end = lineNode;
}

void LineNodeList::Destroy() {
	while(start) {
		LineNode* old = start;
		start = start->next;
		delete old;
		old = NULL;
	}

	start = NULL;
	end = NULL;
}

void LineNodeList::Add(LineNode* lineNode) {
	if(start && end) {
		end->next = lineNode;
		lineNode->prev = end;
		end = lineNode;
	}
	else {
		start = lineNode;
		end = lineNode;
	}
}

void LineNodeList::Remove() {
	LineNode* old = start;
	start = (*start).next;
	delete old;
}
#pragma endregion

// Level
#pragma region Level
Level::Level(int num, float x, float y, int minDeltaY, int maxDeltaY, float w, int minW, int maxW, int screenWidth, int screenHeight, float scrollSpeed) {
	// Store the constant data
	mScrollSpeed = scrollSpeed;
	mMinDeltaY = minDeltaY;
	mMaxDeltaY = maxDeltaY;
	mMinW = maxW - 50;
	mMaxW = maxW;
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// Holds the number of slices
	mNumber = 1;
	
	// Generates the initial slice (always generated)
	mLevelData = new LineNodeList(new LineNode(x - w/2, y, x + w/2, y, NULL, NULL));

	// Generates the rest of the slices minimum of 1, so the level will always start with at least 2 slices
	num--;
	if(num > 0) {
		ExtendLevel(num);
	}
	else {
		ExtendLevel(1);
	}

	mMinW = minW;
}

Level::~Level() {
	delete mLevelData;
}

void Level::ExtendLevel(int num) {
	// Change in position for the next slice
	int deltaY;
	int deltaX;

	// Width and position
	float w = mLevelData->end->GetWidth();
	float x = mLevelData->end->GetMiddleX();
	float y = mLevelData->end->GetMiddleY();

	for(int i = 0; i < num; i++) {
		mNumber++;

		// deltaY is Clamped between minDeltaY and maxDeltaY, which can be changed later to alter the amount of detail in the level
		deltaY = rand() % (mMaxDeltaY-mMinDeltaY) + mMinDeltaY;
		y += deltaY;

		// deltaX is Clamped between -deltaY and deltaY, minimum is 0.5 * deltaY
		deltaX = rand() % (deltaY * 2) - (deltaY);
		deltaX *= 0.9;
		if(deltaX >= 0) {
			deltaX += 0.2 * deltaY;
			if(deltaX < (0.7 * deltaY)) {
				deltaX = 0.5 * deltaY;
			}
		}
		else if(deltaX <= 0) {
			deltaX -= 0.2 * deltaY;
			if(deltaX > (-0.7 * deltaY)) {
				deltaX = -0.5 * deltaY;
			}
		}
		x += deltaX;
		
		// w is clampled between minW and maxW.
		if((rand() % 1) > 0.3)
			w = rand() % (50) + (float)mMinW;
		else
			w = rand() % (mMaxW-mMinW) + (float)mMinW;

		// Keep the passage on the screen with a buffer zone of 100 on either side
		if(x - w/2 < -mScreenWidth - 100)
			x = -mScreenWidth + w/2;
		if(x + w/2 > mScreenWidth + 100)
			x = mScreenWidth - w/2;

		// Generate the new slice
		mLevelData->Add(new LineNode(x - w/2, y, x + w/2, y, NULL, NULL));
	}
}

void Level::CheckLevel() {
	// If the second slice is off screen, remove the first slice
	while((mLevelData->start->next->GetMiddleY()) < 0) {
		mNumber--;
		(*mLevelData).Remove();
	}

	// If the second to last slice is on screen, extend the level by 5 slices
	if((mLevelData->end->prev->GetMiddleY()) < mScreenHeight*2) {
		ExtendLevel(5);
	}
}

CollisionType Level::CheckCollision(SDL_Rect rect) {
	for(LineNode* lineNode = mLevelData->start; lineNode != NULL; lineNode = lineNode->next) {
		// Break if the current slice does not have a next slice
		if( (lineNode->next) == NULL) {
			return NOCOLLISION;
		}
		// Continue until the slice after the current slice is above the bottom of the object
		if( (lineNode->next->GetMiddleY()) < (rect.y) ) {
			continue;
		}
		// Break once the current slice is above the object
		if( (lineNode->GetMiddleY()) > (rect.y + rect.h) ) {
			return NOCOLLISION;
		}

		// Precheck to easily see if object is already inside the wall
		if( ( (rect.x) <= (lineNode->line.x) ) && ( (rect.x) <= (lineNode->next->line.x) ) ) {
			return LEFTWALL;
		}
		if( ( (rect.x + rect.w) >= (lineNode->line.x2) ) && ( (rect.x + rect.w) >= (lineNode->next->line.x2) ) ) {
			return RIGHTWALL;
		}

		// Check if object is close enough to intersect with the walls
		if( ( (lineNode->line.x) > (rect.x) ) || ( (lineNode->next->line.x ) > (rect.x) ) ) {
			if( ( ( (lineNode->line.y) - ( (rect.x + rect.w) - (lineNode->line.x) ) * ( (lineNode->next->line.y - lineNode->line.y)/(lineNode->next->line.x - lineNode->line.x) ) )  <= (rect.y + rect.h) ) && 
			    ( ( (lineNode->line.y) - ( (rect.x + rect.w) - (lineNode->line.x) ) * ( (lineNode->next->line.y - lineNode->line.y)/(lineNode->next->line.x - lineNode->line.x) ) )  >= (rect.y) ) ) {
					return LEFTWALL;
			}
		}
		if( ( (lineNode->line.x2) < (rect.x + rect.w) ) || ( (lineNode->next->line.x2) < (rect.x + rect.w) ) ) {
			if( ( ( (lineNode->line.y2) - ( (rect.x + rect.w) - (lineNode->line.x2) ) * ( (lineNode->next->line.y2 - lineNode->line.y2)/(lineNode->next->line.x2 - lineNode->line.x2) ) )  <= (rect.y + rect.h) ) && 
			    ( ( (lineNode->line.y2) - ( (rect.x + rect.w) - (lineNode->line.x2) ) * ( (lineNode->next->line.y2 - lineNode->line.y2)/(lineNode->next->line.x2 - lineNode->line.x2) ) )  >= (rect.y) ) ) {
					return RIGHTWALL;
			}
		}
	}

	return NOCOLLISION;
}

void Level::Destroy() {
	mLevelData->Destroy();
	delete mLevelData;
}

void Level::Render() {
	glColor3f( 0.f, 0.f, 0.f ); 

	glBegin(GL_QUAD_STRIP);
	for(LineNode* lineNode = mLevelData->start; lineNode != NULL; lineNode = lineNode->next) {
		glVertex2f( -(GLfloat)mScreenWidth , lineNode->line.y  );
		glVertex2f( lineNode->line.x , lineNode->line.y  );
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for(LineNode* lineNode = mLevelData->start; lineNode != NULL; lineNode = lineNode->next) {
		glVertex2f( lineNode->line.x2, lineNode->line.y2 );
		glVertex2f( (GLfloat)mScreenWidth, lineNode->line.y2 );
	}
	glEnd();
}

void Level::Update() {
	CheckLevel();

	LineNode* lineNode = mLevelData->start;

	for(LineNode* lineNode = mLevelData->start; lineNode != NULL; lineNode = lineNode->next) {
		lineNode->line.y  -= mScrollSpeed;
		lineNode->line.y2 -= mScrollSpeed;
	}
}
#pragma endregion