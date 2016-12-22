#ifndef __RETRO_AGENT_HPP__
#define __RETRO_AGENT_HPP__

#include <iostream>
#include "../common/Constants.h"
#include "libretro.h"
#include <atomic>
#ifdef __USE_SDL
#include "SDL.h"
#include "SDL/SDL_rotozoom.h"
#endif

typedef unsigned char byte_t;
class Serializer;
class Deserializer;

namespace rle {

class RetroAgent{
public:
	RetroAgent();
	~RetroAgent();
	RetroAgent(const RetroAgent&) = delete;
	void loadCore(const string& corePath);
	void loadRom(const string& romPath);
	void run();
	void videoRender();
	void swapBuffers();
	void videoDeinit();

	int	 getHeight(); //in pixels
	int	 getWidth(); //in pixels
	void reset();
	int readRam(const int& offset);
	void writeRam(const int& offset, const uint8_t& data);
	uint8_t* getRamAddress();
	uint32_t getRamSize();
	void SetActions(const Action& player_a_action, const Action& player_b_action);
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

	struct g_retro_{
		void *handle;
		bool initialized;

		void (*retro_init)(void);
		void (*retro_deinit)(void);
		unsigned (*retro_api_version)(void);
		void (*retro_get_system_info)(struct retro_system_info *info);
		void (*retro_get_system_av_info)(struct retro_system_av_info *info);
		void (*retro_set_controller_port_device)(unsigned port, unsigned device);
		void (*retro_reset)(void);
		void (*retro_run)(void);
		size_t (*retro_serialize_size)(void);
		bool (*retro_serialize)(void *data, size_t size);
		bool (*retro_unserialize)(const void *data, size_t size);
	//	void retro_cheat_reset(void);
	//	void retro_cheat_set(unsigned index, bool enabled, const char *code);
		bool (*retro_load_game)(const struct retro_game_info *game);
	//	bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info);
		void (*retro_unload_game)(void);
	//	unsigned retro_get_region(void);
		void* (*retro_get_memory_data)(unsigned id);
		size_t (*retro_get_memory_size)(unsigned id);

		int action_a;
		int action_b;
	//	string saveFolder = "/home/administrator/DQN/rle-nano/SNES-Learning-Environment/saves/";
		string corePath;
		size_t serializeSize;
	};
	thread_local static struct g_retro_ g_retro;

	// holds pixel/screen settings
	struct g_video_{
		struct retro_game_geometry rGeom;
	    uint32_t rmask;
	    uint32_t gmask;
	    uint32_t bmask;
	    uint32_t amask;
	    uint32_t bpp;
	    uint32_t pitch;
	    void*    currentBuffer;
	    unsigned format;
	    uint32_t rShift;
	    uint32_t gShift;
	    uint32_t bShift;
	    uint32_t aShift;
	#ifdef __USE_SDL
		SDL_Surface* screen;
	#endif
	};
	thread_local static g_video_ g_video;

	thread_local static unsigned g_joy[2][RETRO_DEVICE_ID_JOYPAD_R3+1];


private:
	bool coreLoaded;
	static std::atomic_uint numAgents;
	unsigned agentNum;
	void unloadCore();
	void unloadRom();
};

} // namespace rle

#endif // __RETRO_AGENT_HPP__
