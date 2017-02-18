/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare, 
 *   Matthew Hausknecht, and the Reinforcement Learning and Artificial Intelligence 
 *   Laboratory
 * Released under the GNU General Public License; see License.txt for details. 
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 *  diplay_screen.cpp 
 *
 *  Supports displaying the screen via SDL. 
 **************************************************************************** */
#include "display_screen.h"
//#include "SoundSDL.hxx"
#include "RetroAgent.h"
#include <iomanip>

using namespace std;
using namespace rle;

#ifdef __USE_SDL
DisplayScreen::DisplayScreen(pRetroAgent rAgent ): manual_control_active(false), delay_msec(5), ragent(rAgent) {

	bpp = ragent->getBpp();
	screen_height = ragent->getHeight();
	screen_width = ragent->getWidth();
    /* Initialize the SDL library and the video subsystem */
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_WM_SetCaption("ALE", NULL);

    /* Clean up on exit */
    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(window_width, window_height, bpp, SDL_SWSURFACE);
    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n", screen_width, screen_height, 8,  SDL_GetError());
        exit(1);
    }

    SDL_WM_SetCaption("ALE Viz", NULL);
    fprintf(stderr, "Screen Display Active. [Manual Control Mode] 'm' "
            "[Slowdown] 'a' [Speedup] 's' [VolumeDown] '[' [VolumeUp] ']'.\n");

    last_frame_time = SDL_GetTicks();
}

DisplayScreen::~DisplayScreen() {
    SDL_Quit();
}


void DisplayScreen::display_screen() {

	if (screen_height != ragent->getHeight() || screen_width != ragent->getWidth()) {
		screen_height = ragent->getHeight() ;
		screen_width = ragent->getWidth() ;
	}
	void* data = ragent->getCurrentBuffer();
	uint32_t pitch = ragent->getPitch();
	uint32_t rmask, gmask, bmask, amask;
	ragent->getRgbMask(rmask, gmask, bmask, amask);

	SDL_Surface * tempScreen = SDL_CreateRGBSurfaceFrom(data, screen_width, screen_height,
								bpp, pitch, rmask, gmask, bmask, 0);
	SDL_Surface* zoomed = zoomSurface(tempScreen, window_width/(double)screen_width, window_height/(double)screen_height, 0);

    screen = SDL_SetVideoMode(window_width, window_height, bpp, SDL_SWSURFACE);
	SDL_BlitSurface(zoomed, NULL, screen, NULL);
	SDL_UpdateRect(screen, 0, 0, screen_width, screen_height);

    SDL_Flip(screen);
    SDL_FreeSurface(tempScreen);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(zoomed);


    poll(); // Check for quit event

    // Wait a while, calibrating so that the delay remains the same
    Uint32 newTime = SDL_GetTicks();
    Uint32 delta = newTime - min(last_frame_time, newTime);

    if (delta < delay_msec) {
        SDL_Delay(delay_msec - delta);
    } else {
        // Try to keep up with the delay
        last_frame_time = SDL_GetTicks() + delta - delay_msec;
    }
}


void DisplayScreen::poll() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        handleSDLEvent(event);
    }
};

void DisplayScreen::handleSDLEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_QUIT:
            exit(0);
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
                case SDLK_m:
                    manual_control_active = !manual_control_active;
                    if (manual_control_active) {
                        fprintf(stderr, "Manual Control Enabled: [Move] "
                                "Arrow keys [A] x [B] z [X] s [Y] a [R] e [L] w [NO-OP] Return.\n");
                    } else {
                        fprintf(stderr, "Manual Control Disabled\n");
                    }
                    break;
                case SDLK_KP_MINUS:
                    delay_msec = delay_msec > 5 ? delay_msec - 5 : 0;
                    fprintf(stderr, "[Speedup] %d msec delay\n", delay_msec);
                    break;
                case SDLK_KP_PLUS:
                    delay_msec = delay_msec + 5;
                    fprintf(stderr, "[Slowdown] %d msec delay\n", delay_msec);
                    break;
#ifdef SOUND_SUPPORT
                case SDLK_LEFTBRACKET:
                    fprintf(stderr, "[VolumeDown]\n");
                    for (int i=0; i<5; ++i) {
//                    	TODO SN: add sound support
//                        ((SoundSDL*)my_sound)->adjustVolume(-1);
                    }
                    break;
                case SDLK_RIGHTBRACKET:
                    fprintf(stderr, "[VolumeUp]\n");
                    for (int i=0; i<5; ++i) {
//                    	TODO SN: add sound support
//                        ((SoundSDL*)my_sound)->adjustVolume(1);
                    }
                    break;
#endif
                default:
                    break;
            }
            break;
        default:
            break;
    }
};

Action DisplayScreen::getUserAction() {
    if (!manual_control_active) {
        return JOYPAD_UNDEFINED;
    }
    Action act = JOYPAD_NOOP;
    poll();
    SDL_PumpEvents();
    Uint8* keymap = SDL_GetKeyState(NULL);
    // Break out of this loop if the 'p' key is pressed
    if (keymap[SDLK_p]) {
      return JOYPAD_NOOP;
    }

    Uint32 up, down, left, right, a, b, x, y, r, l,res;
    up = JOYPAD_UP * keymap[SDLK_UP];
    down = JOYPAD_DOWN * keymap[SDLK_DOWN];
    left = JOYPAD_LEFT * keymap[SDLK_LEFT];
    right = JOYPAD_RIGHT * keymap[SDLK_RIGHT];
    a = JOYPAD_A * keymap[SDLK_x];
    b = JOYPAD_B * keymap[SDLK_z];
    x = JOYPAD_X * keymap[SDLK_s];
    y = JOYPAD_Y * keymap[SDLK_a];
    r = JOYPAD_R * keymap[SDLK_e];
    l = JOYPAD_L * keymap[SDLK_w];
    res = JOYPAD_SYSTEM_RESET * keymap[SDLK_r];
//    DEBUG2("keys pressed: " << a << ", " << b << ", " << x << ", " << y << ", " << r << ", " << l << ", " << up << ", " << down << ", " << left << ", " << right << " , " << res);

    act = up | down | left | right | a | b | x | y | r | l | res;

    return act;
}

#endif // __USE_SDL
