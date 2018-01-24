#ifndef H_ENGINE
#define H_ENGINE

class Player;

class Engine {
private:
	// Screen Surface
	SDL_Surface* mScreen;

	// Window Attributes
	int mScreenWidth;
	int mScreenHeight;
	int mScreenBPP;
	int mFPS;
	std::string mCaption;

	// Game State
	bool mQuit;
	SDL_Event mEvent;

	// Utilities
	Timer mFPSTimer;

	// Main Objects
	Level* mLevel;
	Player* mPlayer;

public:
	Engine(int width, int height, int bpp);
	~Engine();
	void Init();
	void InitOpenGL();
	
	void FPS();
	void Run();

	void GenerateLevel();
	void CreatePlayer();

	void RenderGUI();

	void Update();
	void Render();
};

#else
#pragma message("engine.h superfluously included")
#endif //H_ENGINE