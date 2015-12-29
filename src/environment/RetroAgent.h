
#include<iostream>

// TODO make class singleton
class RetroAgent{
public:
	RetroAgent();
	bool initWindow();
	void loadCore(std::string coreName);
	void unloadCore();
	void loadRom(std::string romName);
	void unloadRom();
	void run();
	bool windowShouldClose();
	void pollEvents();
	void clearWindows();
	void videoRender();
	void swapBuffers();
	void audioDeinit();
	void videoDeinit();
	void terminateWindow();
};
