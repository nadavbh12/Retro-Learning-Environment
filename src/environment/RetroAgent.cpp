#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <dlfcn.h>
#include "Log.hpp"
#include <fstream>

#include <alsa/asoundlib.h>

#include "RleException.h"
#include "RetroAgent.h"
#include "DebugMacros.h"
#include <iomanip>
#include <stdlib.h>
#include "Serializer.hxx"
#include "Deserializer.hxx"

#include <alsa/asoundlib.h>
#include <alsa/control.h>

using namespace rle;

std::atomic_uint RetroAgent::numAgents{0};
thread_local struct RetroAgent::g_retro_ RetroAgent::g_retro;
thread_local struct RetroAgent::g_video_ RetroAgent::g_video;
#ifdef __USE_SDL
thread_local static snd_pcm_t *g_pcm = NULL;
#endif

struct keymap {
	unsigned k;
	unsigned rk;
};

const struct keymap g_binds[] = {
	{ JOYPAD_A		, RETRO_DEVICE_ID_JOYPAD_A 		},
	{ JOYPAD_B		, RETRO_DEVICE_ID_JOYPAD_B 		},
	{ JOYPAD_Y 		, RETRO_DEVICE_ID_JOYPAD_Y 		},
	{ JOYPAD_X 		, RETRO_DEVICE_ID_JOYPAD_X 		},
	{ JOYPAD_R 		, RETRO_DEVICE_ID_JOYPAD_R 		},
	{ JOYPAD_L 		, RETRO_DEVICE_ID_JOYPAD_L 		},
	{ JOYPAD_UP 	, RETRO_DEVICE_ID_JOYPAD_UP 	},
	{ JOYPAD_DOWN 	, RETRO_DEVICE_ID_JOYPAD_DOWN 	},
	{ JOYPAD_LEFT 	, RETRO_DEVICE_ID_JOYPAD_LEFT 	},
	{ JOYPAD_RIGHT 	, RETRO_DEVICE_ID_JOYPAD_RIGHT 	},
	{ JOYPAD_START 	, RETRO_DEVICE_ID_JOYPAD_START 	},
	{ JOYPAD_SELECT	, RETRO_DEVICE_ID_JOYPAD_SELECT },
	{ 0, 0 }
};

thread_local unsigned RetroAgent::g_joy[2][RETRO_DEVICE_ID_JOYPAD_R3+1];

#define load_sym(V, S) do {\
	if (!((*(void**)&V) = dlsym(RetroAgent::g_retro.handle, #S))) \
		die("Failed to load symbol '" #S "'': %s", dlerror()); \
	} while (0)
#define load_retro_sym(S) load_sym(RetroAgent::g_retro.S, S)


static void die(const char *fmt, ...) {
	char buffer[4096];

	va_list va;
	va_start(va, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, va);
	va_end(va);

	fputs(buffer, stderr);
	fputc('\n', stderr);
	fflush(stderr);

	exit(EXIT_FAILURE);
}


static void video_configure(const struct retro_game_geometry *geom) {

	RetroAgent::g_video.rGeom.base_height  = geom->base_height;
	RetroAgent::g_video.rGeom.base_width   = geom->base_width;
	RetroAgent::g_video.rGeom.max_height   = geom->max_height;
	RetroAgent::g_video.rGeom.max_width    = geom->max_width;
	RetroAgent::g_video.rGeom.aspect_ratio = geom->aspect_ratio;
}


static bool video_set_pixel_format(unsigned format) {
//	if (!RetroAgent::g_video.rmask)
//			die("Tried to change pixel format after initialization.");
#ifndef __USE_SDL
    RetroAgent::g_video.format=format;
	switch (format) {
	case RETRO_PIXEL_FORMAT_0RGB1555:
		RetroAgent::g_video.rmask = 0x7c00;
		RetroAgent::g_video.gmask = 0x03e0;
		RetroAgent::g_video.bmask = 0x001f;
		RetroAgent::g_video.amask = 0x0000;
		RetroAgent::g_video.rShift = 10;
		RetroAgent::g_video.gShift = 5;
		RetroAgent::g_video.bShift = 0;
		RetroAgent::g_video.aShift = 15;
		RetroAgent::g_video.bpp = 8*sizeof(uint16_t);
		break;
	case RETRO_PIXEL_FORMAT_XRGB8888:
		RetroAgent::g_video.rmask = 0xff000000;
		RetroAgent::g_video.gmask = 0x00ff0000;
		RetroAgent::g_video.bmask = 0x0000ff00;
		RetroAgent::g_video.amask = 0x000000ff;
		RetroAgent::g_video.rShift = 16;
		RetroAgent::g_video.gShift = 8;
		RetroAgent::g_video.bShift = 0;
		RetroAgent::g_video.aShift = 24;
		RetroAgent::g_video.bpp = 8*sizeof(uint32_t);
		break;
	case RETRO_PIXEL_FORMAT_RGB565:

		RetroAgent::g_video.rmask = 0xf800;
		RetroAgent::g_video.gmask = 0x07e0;
		RetroAgent::g_video.bmask = 0x001f;
		RetroAgent::g_video.amask = 0x0000;
		RetroAgent::g_video.rShift = 11;
		RetroAgent::g_video.gShift = 5;
		RetroAgent::g_video.bShift = 0;
		RetroAgent::g_video.aShift = 16;
		RetroAgent::g_video.bpp = 8*sizeof(uint16_t);
		break;
	default:
		die("Unknown pixel type %u", format);
	}
#endif
	#ifdef __USE_SDL
		// nadav
	    RetroAgent::g_video.format=format;
		switch (format) {
		case RETRO_PIXEL_FORMAT_0RGB1555:
	        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				RetroAgent::g_video.rmask = 0x001f;
				RetroAgent::g_video.gmask = 0x03e0;
				RetroAgent::g_video.bmask = 0x7c00;
				RetroAgent::g_video.amask = 0x0000;
		        RetroAgent::g_video.rShift = 0;
		        RetroAgent::g_video.gShift = 5;
		        RetroAgent::g_video.bShift = 10;
				RetroAgent::g_video.aShift = 15;
	        }else{
				RetroAgent::g_video.rmask = 0x7c00;
				RetroAgent::g_video.gmask = 0x03e0;
				RetroAgent::g_video.bmask = 0x001f;
				RetroAgent::g_video.amask = 0x0000;
		        RetroAgent::g_video.rShift = 10;
		        RetroAgent::g_video.gShift = 5;
		        RetroAgent::g_video.bShift = 0;
				RetroAgent::g_video.aShift = 15;

			}

	        RetroAgent::g_video.rShift = 0;
	        RetroAgent::g_video.gShift = 5;
	        RetroAgent::g_video.bShift = 10;
			RetroAgent::g_video.aShift = 15;
			RetroAgent::g_video.bpp = 8*sizeof(uint16_t);
			break;
		case RETRO_PIXEL_FORMAT_XRGB8888:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				RetroAgent::g_video.rmask = 0x000000ff;
				RetroAgent::g_video.gmask = 0x0000ff00;
				RetroAgent::g_video.bmask = 0x00ff0000;
				RetroAgent::g_video.amask = 0xff000000;
				RetroAgent::g_video.rShift = 0;
				RetroAgent::g_video.gShift = 8;
				RetroAgent::g_video.bShift = 16;
				RetroAgent::g_video.aShift = 24;
			}else{
				RetroAgent::g_video.rmask = 0xff000000;
				RetroAgent::g_video.gmask = 0x00ff0000;
				RetroAgent::g_video.bmask = 0x0000ff00;
				RetroAgent::g_video.amask = 0x000000ff;
				RetroAgent::g_video.rShift = 24;
				RetroAgent::g_video.gShift = 16;
				RetroAgent::g_video.bShift = 8;
				RetroAgent::g_video.aShift = 0;
			}

			RetroAgent::g_video.bpp = 8*sizeof(uint32_t);
			break;
		case RETRO_PIXEL_FORMAT_RGB565:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				RetroAgent::g_video.rmask = 0x001f;
				RetroAgent::g_video.gmask = 0x07e0;
				RetroAgent::g_video.bmask = 0xf800;
				RetroAgent::g_video.amask = 0x0000;
				RetroAgent::g_video.rShift = 0;
				RetroAgent::g_video.gShift = 5;
				RetroAgent::g_video.bShift = 11;
				RetroAgent::g_video.aShift = 0;
			}else{
				RetroAgent::g_video.rmask = 0xf800;
				RetroAgent::g_video.gmask = 0x07e0;
				RetroAgent::g_video.bmask = 0x001f;
				RetroAgent::g_video.amask = 0x0000;
				RetroAgent::g_video.rShift = 11;
				RetroAgent::g_video.gShift = 5;
				RetroAgent::g_video.bShift = 0;
				RetroAgent::g_video.aShift = 0;
			}

			RetroAgent::g_video.bpp = 8*sizeof(uint16_t);
			break;
		default:
			die("Unknown pixel type %u", format);
		}
#endif
	return true;
}


static void video_refresh(const void *data, unsigned width, unsigned height, unsigned pitch) {
	if (RetroAgent::g_video.rGeom.base_width != width || RetroAgent::g_video.rGeom.base_height != height) {
		RetroAgent::g_video.rGeom.base_height = height;
		RetroAgent::g_video.rGeom.base_width  = width;
	}
	RetroAgent::g_video.pitch = pitch;
	RetroAgent::g_video.currentBuffer = const_cast<void*>(data);
}

static void audio_init(int frequency) {
#ifdef __USE_SDL
	int err;

	if ((err = snd_pcm_open(&g_pcm, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
		die("Failed to open playback device: %s", snd_strerror(err));

	err = snd_pcm_set_params(g_pcm, SND_PCM_FORMAT_S16, SND_PCM_ACCESS_RW_INTERLEAVED, 2, frequency, 1, 64 * 1000);

	if (err < 0)
		die("Failed to configure playback device: %s", snd_strerror(err));
#endif
}

static void audio_deinit() {
#ifdef __USE_SDL
	snd_pcm_close(g_pcm);
#endif
}

static size_t audio_write(const void *buf, unsigned frames) {
#ifdef __USE_SDL
	int written = snd_pcm_writei(g_pcm, buf, frames);

	if (written < 0) {
		printf("Alsa warning/error #%i: ", -written);
		snd_pcm_recover(g_pcm, written, 0);

		return 0;
	}

	return written;
#endif
	return 0;
}

static void core_log(enum retro_log_level level, const char *fmt, ...) {
	char buffer[4096] = {0};
	static const char * levelstr[] = { "dbg", "inf", "wrn", "err" };
	va_list va;

	va_start(va, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, va);
	va_end(va);

	if (level == 0)
		return;

	fprintf(stderr, "[%s] %s", levelstr[level], buffer);
	fflush(stderr);

	if (level == RETRO_LOG_ERROR)
		exit(EXIT_FAILURE);
}


static bool core_environment(unsigned cmd, void *data) {
	bool *bval;

	switch (cmd) {
	case RETRO_ENVIRONMENT_GET_LOG_INTERFACE: {
		struct retro_log_callback *cb = (struct retro_log_callback *)data;
		cb->log = core_log;
		break;
	}
	case RETRO_ENVIRONMENT_GET_CAN_DUPE:
		bval = (bool*)data;
		*bval = true;
		break;
	case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT: {
		const enum retro_pixel_format *fmt = (enum retro_pixel_format *)data; // RETRO_PIXEL_FORMAT_XRGB8888 is taken

		if (*fmt > RETRO_PIXEL_FORMAT_RGB565){
			return false;
		}
		return video_set_pixel_format(*fmt);
	}
//	case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY :{
//		const char* cval = (char*)data;
//		cval = RetroAgent::g_retro.saveFolder.c_str();
//		return true;
//	}
	case RETRO_ENVIRONMENT_GET_VARIABLE:{
		struct retro_variable* r_var = (struct retro_variable*)data;
		if(!strcmp(r_var->key, "genesis_plus_gx_bram")){
			r_var->value = "";
		}else if(!strcmp(r_var->key, "genesis_plus_gx_bram")){
			r_var->value = "auto";
		}else if(!strcmp(r_var->key, "genesis_plus_gx_blargg_ntsc_filter")){
			r_var->value = "rgb";
		}

	}
	default:
		core_log(RETRO_LOG_DEBUG, "Unhandled env #%u", cmd);
		return false;
	}

	return true;
}

static void core_video_refresh(const void *data, unsigned width, unsigned height, size_t pitch) {
	if (data)
		video_refresh(data, width, height, pitch);
}

static void core_input_poll(void) {
	int i;
	for (i = 0; g_binds[i].k || g_binds[i].rk; ++i){
		RetroAgent::g_joy[0][g_binds[i].rk] = (RetroAgent::g_retro.action_a & g_binds[i].k) > 0;
//		if(RetroAgent::g_joy[0][g_binds[i].rk]) DEBUG2("PLAYER A " << action_to_string(g_binds[i].k) << " = " << RetroAgent::g_joy[1][g_binds[i].rk])
	}
	for (i = 0; g_binds[i].k || g_binds[i].rk; ++i){
		RetroAgent::g_joy[1][g_binds[i].rk] = (RetroAgent::g_retro.action_b & g_binds[i].k) > 0;
//		if(RetroAgent::g_joy[1][g_binds[i].rk]) DEBUG2("PLAYER B " << action_to_string(g_binds[i].k) << " = " << RetroAgent::g_joy[1][g_binds[i].rk])

	}

}

// port == player number
static int16_t core_input_state(unsigned port, unsigned device, unsigned index, unsigned id) {
	if (index || device != RETRO_DEVICE_JOYPAD)
		return 0;
	return RetroAgent::g_joy[port][id];
}

static void core_audio_sample(int16_t left, int16_t right) {
	int16_t buf[2] = {left, right};
	if(RetroAgent::g_retro.audioEnabled){
		audio_write(buf, 1);
	}
}


static size_t core_audio_sample_batch(const int16_t *data, size_t frames) {
	if(RetroAgent::g_retro.audioEnabled){
		return audio_write(data, frames);
	}else{
		return 0;
	}
}

static void core_load(const char *sofile) {
	void (*set_environment)(retro_environment_t) = NULL;
	void (*set_video_refresh)(retro_video_refresh_t) = NULL;
	void (*set_input_poll)(retro_input_poll_t) = NULL;
	void (*set_input_state)(retro_input_state_t) = NULL;
	void (*set_audio_sample)(retro_audio_sample_t) = NULL;
	void (*set_audio_sample_batch)(retro_audio_sample_batch_t) = NULL;

	RetroAgent::g_retro.handle = dlopen(sofile, RTLD_LAZY);
	RetroAgent::g_retro.corePath= sofile;

	if (!RetroAgent::g_retro.handle)
		die("Failed to load core: %s", dlerror());

	dlerror();

	load_retro_sym(retro_init);
	load_retro_sym(retro_deinit);
	load_retro_sym(retro_api_version);
	load_retro_sym(retro_get_system_info);
	load_retro_sym(retro_get_system_av_info);
	load_retro_sym(retro_set_controller_port_device);
	load_retro_sym(retro_run);
	load_retro_sym(retro_serialize_size);
	load_retro_sym(retro_serialize);
	load_retro_sym(retro_unserialize);
	load_retro_sym(retro_load_game);
	load_retro_sym(retro_unload_game);
	load_retro_sym(retro_reset);
	load_retro_sym(retro_get_memory_data);
	load_retro_sym(retro_get_memory_size);

	load_sym(set_environment, retro_set_environment);
	load_sym(set_video_refresh, retro_set_video_refresh);
	load_sym(set_input_poll, retro_set_input_poll);
	load_sym(set_input_state, retro_set_input_state);
	load_sym(set_audio_sample, retro_set_audio_sample);
	load_sym(set_audio_sample_batch, retro_set_audio_sample_batch);

	set_environment(core_environment);
	set_video_refresh(core_video_refresh);
	set_input_poll(core_input_poll);
	set_input_state(core_input_state);
	set_audio_sample(core_audio_sample);
	set_audio_sample_batch(core_audio_sample_batch);

	RetroAgent::g_retro.retro_init();
	RetroAgent::g_retro.initialized = true;

	puts("Core loaded");
}

static void core_load_game(const char *filename) {
	struct retro_system_av_info av = {0};
	struct retro_system_info system = {0};
	struct retro_game_info info = { filename, 0 };
	FILE *file = fopen(filename, "rb");

	if (!file)
		goto libc_error;

	fseek(file, 0, SEEK_END);
	info.size = ftell(file);
	rewind(file);

	RetroAgent::g_retro.retro_get_system_info(&system);

	if (!system.need_fullpath) {
		info.data = malloc(info.size);

		if (!info.data || !fread((void*)info.data, info.size, 1, file))
			goto libc_error;
	}

	if (!RetroAgent::g_retro.retro_load_game(&info))
		die("The core failed to load the content.");

	RetroAgent::g_retro.retro_get_system_av_info(&av);

	video_configure(&av.geometry);
	audio_init(av.timing.sample_rate);

	free((void*)info.data);

	return;

libc_error:
	die("Failed to load content '%s': %s", filename, strerror(errno));
}

static void core_unload() {
	if (RetroAgent::g_retro.initialized)
		RetroAgent::g_retro.retro_deinit();
//	if (RetroAgent::g_retro.handle)
//		dlclose(RetroAgent::g_retro.handle);
}

RetroAgent::RetroAgent() : coreLoaded(false), romLoaded(false){
	agentNum = numAgents++;
	RetroAgent::g_retro.initialized = false;
}

RetroAgent::~RetroAgent(){
	unloadRom();
	core_unload();
}

static bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

static void copyFile(string srcName, string dstName){
    std::ifstream  src(srcName, std::ios::binary);
    std::ofstream  dst(dstName,   std::ios::binary);

    dst << src.rdbuf();
}

void RetroAgent::loadCore(const string& coreName){
	if(coreLoaded){
		unloadCore();
	}
	string suffix = ".so";
	size_t start_pos = coreName.find(suffix);
	if(start_pos == std::string::npos){
		throw invalid_argument("Invalid core file path: " + coreName +
				+"\n File name must end with .so");
	}
	if(agentNum > 0){
		string newCoreName = coreName;
		string newSuffix = std::to_string(agentNum) + suffix;
		replace(newCoreName, suffix, newSuffix);
		if(!std::ifstream(newCoreName)){ // if doesn't file exist
			copyFile(coreName,newCoreName);
		}
		core_load(newCoreName.c_str());
	}
	else{
		core_load(coreName.c_str());
	}
	coreLoaded = true;
}

void RetroAgent::unloadCore(){
	core_unload();
	audio_deinit();
	coreLoaded = false;
}

void RetroAgent::loadRom(const string& romName){
	if(romLoaded){
		unloadRom();
	}
	core_load_game(romName.c_str());
	RetroAgent::g_retro.serializeSize = RetroAgent::g_retro.retro_serialize_size();
	romLoaded = true;
}

void RetroAgent::unloadRom(){
	if (RetroAgent::g_retro.initialized){
		RetroAgent::g_retro.retro_unload_game();
		RetroAgent::g_retro.initialized = false;
	}
	romLoaded = false;
}

void RetroAgent::run(){
	RetroAgent::g_retro.retro_run();
}

void RetroAgent::audioEnable(bool audioState){
	RetroAgent::g_retro.audioEnabled = audioState;
}

int RetroAgent::getHeight(){
	return RetroAgent::g_video.rGeom.base_height;
}

int RetroAgent::getWidth(){
	return RetroAgent::g_video.rGeom.base_width; //in  pix
}

void RetroAgent::reset(){
	g_retro.retro_reset();
}

int RetroAgent::readRam(const int& offset){
	assert( (uint32_t)offset < getRamSize());
	assert( offset > 0);
	assert( coreLoaded );
	return *(getRamAddress() + offset);
}

void RetroAgent::writeRam(const int& offset, const uint8_t& data){
	assert( (uint32_t)offset < getRamSize());
	assert( offset > 0);
	assert( coreLoaded );
	*(getRamAddress() + offset) = data;
}

uint8_t* RetroAgent::getRamAddress(){
	assert( coreLoaded );
	   size_t size = g_retro.retro_get_memory_size(RETRO_MEMORY_SYSTEM_RAM);
	   void*  data = g_retro.retro_get_memory_data(RETRO_MEMORY_SYSTEM_RAM);
	   if (!size){
		   throw RleException("Ram size is 0");
	   }else{
		   return (uint8_t*)data;
	   }
}

uint32_t RetroAgent::getRamSize(){
	assert( coreLoaded );
	return (uint32_t)g_retro.retro_get_memory_size(RETRO_MEMORY_SYSTEM_RAM);
}

void RetroAgent::SetActions(const Action& player_a_action, const Action& player_b_action){
	g_retro.action_a = player_a_action;
	g_retro.action_b = player_b_action;
}

void* RetroAgent::getCurrentBuffer() const{
	return RetroAgent::g_video.currentBuffer;
}

uint8_t RetroAgent::getBpp() const{
	return RetroAgent::g_video.bpp;
}

uint32_t RetroAgent::getPitch() const{
	return RetroAgent::g_video.pitch;
}

unsigned RetroAgent::getFormat()const{
	return RetroAgent::g_video.format;
}

void RetroAgent::getRgbMask(uint32_t& rmask, uint32_t& gmask, uint32_t& bmask, uint32_t& amask) const{
	rmask = RetroAgent::g_video.rmask;
	gmask = RetroAgent::g_video.gmask;
	bmask = RetroAgent::g_video.bmask;
	amask = RetroAgent::g_video.amask;
}

uint32_t RetroAgent::getBufferSize() const{
	return RetroAgent::g_video.rGeom.base_width * RetroAgent::g_video.rGeom.base_height;

}

void RetroAgent::getRgbShift(uint32_t& rShift, uint32_t& gShift, uint32_t& bShift, uint32_t &aShift) const{
	rShift=RetroAgent::g_video.rShift;
	gShift=RetroAgent::g_video.gShift;
	bShift=RetroAgent::g_video.bShift;
	aShift=RetroAgent::g_video.aShift;

}

void RetroAgent::getRgb (const uint32_t& pixel, uint8_t &r, uint8_t &g ,uint8_t &b ) const{
	uint32_t rmask , bmask, gmask, amask;
	uint32_t rShift, bShift, gShift,aShift;
	getRgbMask( rmask,  gmask,  bmask,  amask);
	getRgbShift(rShift, gShift, bShift, aShift);

	r=(pixel & rmask) >> rShift;
	g=(pixel & gmask) >> gShift;
	b=(pixel & bmask) >> bShift;
//	a=(pixel & amask) >> aShift;
}

void RetroAgent::serialize(Serializer& ser){
	g_retro.serializeSize = g_retro.retro_serialize_size();

	//	sizeof(char)*g_retro.serializeSize
	void* data = malloc(16 * sizeof(char)*g_retro.serializeSize);
	g_retro.retro_serialize(data,g_retro.serializeSize);
	size_t intSize = g_retro.serializeSize * sizeof(int)/sizeof(uint8_t);
	ser.putIntArray((const int*)data, intSize);
	free(data);
}

void RetroAgent::deserialize(Deserializer& des){
	g_retro.serializeSize = g_retro.retro_serialize_size();
	void* data = malloc(16 * sizeof(char)*g_retro.serializeSize);
	size_t intSize = g_retro.serializeSize * sizeof(int)/sizeof(uint8_t);
	des.getIntArray((int*)data, intSize);
	g_retro.retro_unserialize(data, g_retro.serializeSize);
	free(data);
}
