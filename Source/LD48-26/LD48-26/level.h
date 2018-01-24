#ifndef H_LEVEL
#define H_LEVEL

//Collision Types
enum CollisionType { LEFTWALL, RIGHTWALL, NOCOLLISION};

// Line
struct Line {
	float x;
	float y;
	float x2;
	float y2;
};

// Line Node
class LineNode {
public:
	Line line;			// Structure holding the line data representing the horizontal slice
	LineNode* prev;		// Address to the previous slice (below)
	LineNode* next;		// Address to the next slice (above)
	LineNode(float x, float y, float x2, float y2, LineNode* before, LineNode* after);
	float GetWidth();	// Returns the slice's position width value
	float GetMiddleX();	// Returns the slice's position x value
	float GetMiddleY();	// Returns the slice's position y value
};

// Line Node List
class LineNodeList {
public:
	LineNode* start;					// Address to the first node (the bottom-most horizontal slice)
	LineNode* end;						// Address to the last node (the top-most horizontal slice)
	LineNodeList(LineNode* lineNode);
	void Destroy();
	void Add(LineNode* lineNode);		// Add a slice to the end (top)
	void Remove();						// Remove a slice from the start (bottom)
};

// Level
class Level {
private:
	LineNodeList* mLevelData;			// Holds the level slice data for the level
	int mNumber;						// Number of horizontal slices
	float mScrollSpeed;					// Scroll speed of the level (allways scrolls down the Y axis)
	int mScreenWidth, mScreenHeight;	// The width and height of the screen
	int mMinDeltaY, mMaxDeltaY;			// The minimum and maximum delta Y between each vertical slice
	int mMinW, mMaxW;					// The minimum and maximum slice width
public:
	Level(int num, float x, float y, int minDeltaY, int maxDeltaY, float w, int minW, int maxW, int screenWidth, int screenHeight, float scrollSpeed);
	~Level();
	void ExtendLevel(int num);
	void CheckLevel();
	CollisionType CheckCollision(SDL_Rect rect);
	void Destroy();
	void Render();
	void Update();
};

#else
#pragma message("level.h superfluously included")
#endif //H_LEVEL