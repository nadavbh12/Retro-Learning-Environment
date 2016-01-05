#ifndef __RETRO_AGENT_HPP__
#define __RETRO_AGENT_HPP__

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
//	void audioInit();	// TODO SN: get implementation from loadRom
//	void videoInit();	// TODO same as above
	void audioDeinit();
	void videoDeinit();
	void terminateWindow();

//	Below function were added by demand
	int	 getHeight();
	int	 getWidth();
	void reset();
	int readRam(unsigned id, int offset);
};

#endif // __RETRO_AGENT_HPP__
