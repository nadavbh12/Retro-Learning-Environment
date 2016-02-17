
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <dlfcn.h>

#include "libretro.h"

// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
#include <alsa/asoundlib.h>
#ifdef __USE_SDL
#include "SDL.h"
#include "SDL/SDL_rotozoom.h"
#endif

#include "AleException.h"
#include "RetroAgent.h"
#include "DebugMacros.h"
#include <iomanip>
#include <stdlib.h>

using namespace ale;

static void printRam(const void* data, size_t size){
	system("clear");
	uint8_t* nData = (uint8_t*)data;
   cout << "------------------------------------------------------" << endl;
   cout << "      ";
   for (int i=0; i<16; i++){
	   cout << std::setw(1) << std::hex << i << "  ";
   }
   for (uint8_t i = 0; i < size; i++){
//	   cerr << +i << endl;
	   if(i%16 == 0){
		   cout << endl << setw(4) << std::hex << std::setfill('0') << +i << " ";
	   }
	   cout << std::hex << std::setw(2) << std::setfill('0')<<  +nData[i] << " ";
   }
   cout << endl;
}

// holds pixel/screen settings
static struct {
	// nadav implementations below:
	struct retro_game_geometry rGeom;
    uint32_t rmask;
    uint32_t gmask;
    uint32_t bmask;
    uint32_t amask;
    uint32_t bpp;
    uint32_t pitch;
    void*    currentBuffer;
#ifdef __USE_SDL
	SDL_Surface* screen;
#endif
} g_video  = {0};

static struct {
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
//	size_t retro_serialize_size(void);
//	bool retro_serialize(void *data, size_t size);
//	bool retro_unserialize(const void *data, size_t size);
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
	uint8_t* currentBuffer;
//	void* currentBuffer;

} g_retro;


struct keymap {
	unsigned k;
	unsigned rk;
};

struct keymap g_binds[] = {
	{ JOYPAD_A		, RETRO_DEVICE_ID_JOYPAD_A 		},
	{ JOYPAD_B		, RETRO_DEVICE_ID_JOYPAD_B 		},
	{ JOYPAD_Y 		, RETRO_DEVICE_ID_JOYPAD_Y 		},
	{ JOYPAD_X 		, RETRO_DEVICE_ID_JOYPAD_X 		},
	{ JOYPAD_UP 	, RETRO_DEVICE_ID_JOYPAD_UP 	},
	{ JOYPAD_DOWN 	, RETRO_DEVICE_ID_JOYPAD_DOWN 	},
	{ JOYPAD_LEFT 	, RETRO_DEVICE_ID_JOYPAD_LEFT 	},
	{ JOYPAD_RIGHT 	, RETRO_DEVICE_ID_JOYPAD_RIGHT 	},
	{ JOYPAD_START 	, RETRO_DEVICE_ID_JOYPAD_START 	},
	{ JOYPAD_SELECT	, RETRO_DEVICE_ID_JOYPAD_SELECT },
	{ 0, 0 }
};

static unsigned g_joy[RETRO_DEVICE_ID_JOYPAD_R3+1] = { 0 };

#define load_sym(V, S) do {\
	if (!((*(void**)&V) = dlsym(g_retro.handle, #S))) \
		die("Failed to load symbol '" #S "'': %s", dlerror()); \
	} while (0)
#define load_retro_sym(S) load_sym(g_retro.S, S)


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

	g_video.rGeom.base_height  = geom->base_height;
	g_video.rGeom.base_width   = geom->base_width;
	g_video.rGeom.max_height   = geom->max_height;
	g_video.rGeom.max_width    = geom->max_width;
	g_video.rGeom.aspect_ratio = geom->aspect_ratio;
}


static bool video_set_pixel_format(unsigned format) {
//	if (!g_video.rmask)
//			die("Tried to change pixel format after initialization.");
#ifdef __USE_SDL
		// nadav
		switch (format) {
		case RETRO_PIXEL_FORMAT_0RGB1555:
	        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				g_video.rmask = 0x001f;
				g_video.gmask = 0x03e0;
				g_video.bmask = 0x7c00;
				g_video.amask = 0x0000;
	        }else{
				g_video.rmask = 0x7c00;
				g_video.gmask = 0x03e0;
				g_video.bmask = 0x001f;
				g_video.amask = 0x0000;
			}
			g_video.bpp = 8*sizeof(uint16_t);
			break;
		case RETRO_PIXEL_FORMAT_XRGB8888:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				g_video.rmask = 0x000000ff;
				g_video.gmask = 0x0000ff00;
				g_video.bmask = 0x00ff0000;
				g_video.amask = 0xff000000;
			}else{
				g_video.rmask = 0xff000000;
				g_video.gmask = 0x00ff0000;
				g_video.bmask = 0x0000ff00;
				g_video.amask = 0x000000ff;
			}
			g_video.bpp = 8*sizeof(uint32_t);
			break;
		case RETRO_PIXEL_FORMAT_RGB565:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				g_video.rmask = 0x001f;
				g_video.gmask = 0x07e0;
				g_video.bmask = 0xf800;
				g_video.amask = 0x0000;
			}else{
				g_video.rmask = 0xf800;
				g_video.gmask = 0x07e0;
				g_video.bmask = 0x001f;
				g_video.amask = 0x0000;
			}
			g_video.bpp = 8*sizeof(uint16_t);
			break;
		default:
			die("Unknown pixel type %u", format);
		}
#endif
	return true;
}


static void video_refresh(const void *data, unsigned width, unsigned height, unsigned pitch) {
	if (g_video.rGeom.base_width != width || g_video.rGeom.base_height != height) {
		g_video.rGeom.base_height = height;
		g_video.rGeom.base_width  = width;
	}
	g_video.pitch = pitch;
	g_video.currentBuffer = const_cast<void*>(data);
}

static void audio_init(int frequency) {
}


static void audio_deinit() {
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
			//cout << "False Returned" << endl;
			return false;
		}
		return video_set_pixel_format(*fmt);

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
	for (i = 0; g_binds[i].k || g_binds[i].rk; ++i)
		g_joy[g_binds[i].rk] = (g_retro.action_a & g_binds[i].k) > 0;
}


static int16_t core_input_state(unsigned port, unsigned device, unsigned index, unsigned id) {
	// player b will be supported by a different port
	if (port || index || device != RETRO_DEVICE_JOYPAD)
		return 0;
//	DEBUG2("Value of " << id << " is: " << g_joy[id]);
	return g_joy[id];
}


static void core_audio_sample(int16_t left, int16_t right) {
}


static size_t core_audio_sample_batch(const int16_t *data, size_t frames) {
}


static void core_load(const char *sofile) {
	void (*set_environment)(retro_environment_t) = NULL;
	void (*set_video_refresh)(retro_video_refresh_t) = NULL;
	void (*set_input_poll)(retro_input_poll_t) = NULL;
	void (*set_input_state)(retro_input_state_t) = NULL;
	void (*set_audio_sample)(retro_audio_sample_t) = NULL;
	void (*set_audio_sample_batch)(retro_audio_sample_batch_t) = NULL;

	memset(&g_retro, 0, sizeof(g_retro));
	g_retro.handle = dlopen(sofile, RTLD_LAZY);

	if (!g_retro.handle)
		die("Failed to load core: %s", dlerror());

	dlerror();

	load_retro_sym(retro_init);
	load_retro_sym(retro_deinit);
	load_retro_sym(retro_api_version);
	load_retro_sym(retro_get_system_info);
	load_retro_sym(retro_get_system_av_info);
	load_retro_sym(retro_set_controller_port_device);
	load_retro_sym(retro_run);
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

	g_retro.retro_init();
	g_retro.initialized = true;

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

	g_retro.retro_get_system_info(&system);

	if (!system.need_fullpath) {
		info.data = malloc(info.size);

		if (!info.data || !fread((void*)info.data, info.size, 1, file))
			goto libc_error;
	}

	if (!g_retro.retro_load_game(&info))
		die("The core failed to load the content.");

	g_retro.retro_get_system_av_info(&av);

	video_configure(&av.geometry);
	audio_init(av.timing.sample_rate);

	return;

libc_error:
	die("Failed to load content '%s': %s", filename, strerror(errno));
}


static void core_unload() {
	if (g_retro.initialized)
		g_retro.retro_deinit();
}

RetroAgent::RetroAgent(){}

RetroAgent::~RetroAgent(){
	core_unload();
}


void RetroAgent::loadCore(std::string coreName){
	core_load(coreName.c_str());
}

void RetroAgent::unloadCore(){
	core_unload();
}
void RetroAgent::loadRom(std::string romName){
	core_load_game(romName.c_str());
}

void RetroAgent::unloadRom(){
	core_unload();
}

void RetroAgent::run(){
	g_retro.retro_run();
}

int RetroAgent::getHeight(){
	return g_video.rGeom.base_height;
}

int RetroAgent::getWidth(){
	return g_video.rGeom.base_width;

}

void RetroAgent::reset(){
	g_retro.retro_reset();
}



int RetroAgent::readRam(unsigned id, int offset){
//	unsigned* data = (unsigned*)g_retro.retro_get_memory_data(id);
//	return data[offset];
   size_t size = g_retro.retro_get_memory_size(id);
   void*  data = g_retro.retro_get_memory_data(id);
//   printRam(data,size);
   if (!size){
	   throw AleException("Ram size is 0");
   }else if((unsigned)offset > (size-1)){
	   throw AleException("Offset is larger than RAM size");
   }else{
	   return *((char*)data+offset);
   }
}

// TODO SN :  currently only one player is supported
void RetroAgent::SetActions(int player_a_action, int player_b_action){
	g_retro.action_a = player_a_action;
	//DEBUG2("g_retro.action_a is: " << action_to_string(g_retro.action_a));
	g_retro.action_b = player_b_action;
}

void RetroAgent::updateScreen(){

}
void* RetroAgent::getCurrentBuffer(){
	return g_video.currentBuffer;
}

uint8_t RetroAgent::getBpp() const{
	return g_video.bpp;
}

uint32_t RetroAgent::getPitch() const{
	return g_video.pitch;
}

void RetroAgent::getRgbMask(uint32_t& rmask, uint32_t& gmask, uint32_t& bmask, uint32_t& amask) const{
	rmask = g_video.rmask;
	gmask = g_video.gmask;
	bmask = g_video.bmask;
	amask = g_video.amask;
}
