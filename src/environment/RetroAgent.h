#ifndef __RETRO_AGENT_HPP__
#define __RETRO_AGENT_HPP__

#include <iostream>
#include "../common/Constants.h"

typedef unsigned char byte_t;
class Serializer;
class Deserializer;

namespace ale {

class RetroAgent{
public:
	RetroAgent();
	~RetroAgent();
	void loadCore(std::string corePath);
	void loadRom(std::string romPath);
	void run();
	void videoRender();
	void swapBuffers();
	void videoDeinit();

//	Below function were added by demand
	int	 getHeight(); //in pixels
	int	 getWidth(); //in pixels
	void reset();
	int readRam(unsigned id, int offset);
	uint8_t* getRamAddress(unsigned id);
	uint32_t getRamSize();
	void SetActions(int player_a_action, int player_b_action);
	void updateScreen();
	void* getCurrentBuffer() const;
	uint32_t getBufferSize() const;	// in pixels
	uint8_t getBpp() const; //in bits
	uint32_t getPitch() const;
	void getRgbMask(uint32_t& rmask, uint32_t& gmask, uint32_t& bmask, uint32_t& amask) const;
	void getRgbShift(uint32_t& rShift, uint32_t& gShift, uint32_t& bShift, uint32_t &aShift) const;
	void getRgb (const uint32_t& pixel, uint8_t &r, uint8_t &g ,uint8_t &b) const;
	unsigned getFormat()const;
	void serialize(Serializer& ser);
	void deserialize(Deserializer& des);

private:
	void unloadCore();
	void unloadRom();
};

} // namespace ale

#endif // __RETRO_AGENT_HPP__
