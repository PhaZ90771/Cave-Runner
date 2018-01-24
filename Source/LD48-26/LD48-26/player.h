#ifndef H_PLAYER
#define H_PLAYER

struct Input {
	bool left;
	bool right;
	bool up;
	bool down;
};

class Player {
private:
	float mX, mY, mW, mH;
	float mMoveSpeed;
	float mDX, mDY;
	int mScreenWidth, mScreenHeight;
	Input mInput;
	int mHP;
	Uint32 mNextHitTime;
	int mHitDelay;
	bool mHit;
	bool mFlashColor;
	Uint32 mNextFlashTime;
	int mFlashDelay;
public:
	Player(float x, float y, int screenWidth, int screenHeight);
	SDL_Rect GetPosition();
	void SetLeft(bool left);
	void SetRight(bool right);
	void SetUp(bool up);
	void SetDown(bool down);
	int GetHP() { return mHP; };
	void Move();
	void UpdateSpeed();
	void Bump(float x) ;
	void Hit();
	bool Update();
	void Render();
};

#else
#pragma message("player.h superfluously included")
#endif //H_PLAYER