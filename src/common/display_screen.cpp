/* *****************************************************************************#include "SDL.h
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
#include "SoundSDL.hxx"
#include "RetroAgent.h"
#include <iomanip>

using namespace std;
using namespace ale;

static uint32_t NTSCPalette[256] = {
    0x000000,0x000000,0x4a4a4a,0x4a4a4a,0x6f6f6f,0x6f6f6f,0x8e8e8e,0x8e8e8e,
    0xaaaaaa,0xaaaaaa,0xc0c0c0,0xc0c0c0,0xd6d6d6,0xd6d6d6,0xececec,0xececec,
    0x484800,0x404040,0x69690f,0x5f5f5f,0x86861d,0x7a7a7a,0xa2a22a,0x949494,
    0xbbbb35,0xacacac,0xd2d240,0xc1c1c1,0xe8e84a,0xd6d6d6,0xfcfc54,0xe9e9e9,
    0x7c2c00,0x3f3f3f,0x904811,0x575757,0xa26221,0x6e6e6e,0xb47a30,0x838383,
    0xc3903d,0x969696,0xd2a44a,0xa7a7a7,0xdfb755,0xb8b8b8,0xecc860,0xc7c7c7,
    0x901c00,0x3b3b3b,0xa33915,0x555555,0xb55328,0x6b6b6b,0xc66c3a,0x818181,
    0xd5824a,0x949494,0xe39759,0xa7a7a7,0xf0aa67,0xb7b7b7,0xfcbc74,0xc7c7c7,
    0x940000,0x2c2c2c,0xa71a1a,0x444444,0xb83232,0x5a5a5a,0xc84848,0x6e6e6e,
    0xd65c5c,0x808080,0xe46f6f,0x929292,0xf08080,0xa1a1a1,0xfc9090,0xb0b0b0,
    0x840064,0x333333,0x97197a,0x4a4a4a,0xa8308f,0x5f5f5f,0xb846a2,0x737373,
    0xc659b3,0x848484,0xd46cc3,0x959595,0xe07cd2,0xa4a4a4,0xec8ce0,0xb2b2b2,
    0x500084,0x272727,0x68199a,0x3f3f3f,0x7d30ad,0x555555,0x9246c0,0x6b6b6b,
    0xa459d0,0x7d7d7d,0xb56ce0,0x8f8f8f,0xc57cee,0x9f9f9f,0xd48cfc,0xaeaeae,
    0x140090,0x161616,0x331aa3,0x313131,0x4e32b5,0x494949,0x6848c6,0x606060,
    0x7f5cd5,0x747474,0x956fe3,0x888888,0xa980f0,0x999999,0xbc90fc,0xa9a9a9,
    0x000094,0x111111,0x181aa7,0x292929,0x2d32b8,0x404040,0x4248c8,0x555555,
    0x545cd6,0x686868,0x656fe4,0x797979,0x7580f0,0x898989,0x8490fc,0x999999,
    0x001c88,0x202020,0x183b9d,0x3c3c3c,0x2d57b0,0x555555,0x4272c2,0x6d6d6d,
    0x548ad2,0x828282,0x65a0e1,0x969696,0x75b5ef,0xa8a8a8,0x84c8fc,0xbababa,
    0x003064,0x282828,0x185080,0x454545,0x2d6d98,0x5f5f5f,0x4288b0,0x787878,
    0x54a0c5,0x8d8d8d,0x65b7d9,0xa2a2a2,0x75cceb,0xb6b6b6,0x84e0fc,0xc8c8c8,
    0x004030,0x2b2b2b,0x18624e,0x4a4a4a,0x2d8169,0x656565,0x429e82,0x7f7f7f,
    0x54b899,0x979797,0x65d1ae,0xadadad,0x75e7c2,0xc1c1c1,0x84fcd4,0xd4d4d4,
    0x004400,0x282828,0x1a661a,0x474747,0x328432,0x626262,0x48a048,0x7c7c7c,
    0x5cba5c,0x939393,0x6fd26f,0xa9a9a9,0x80e880,0xbdbdbd,0x90fc90,0xcfcfcf,
    0x143c00,0x292929,0x355f18,0x4a4a4a,0x527e2d,0x686868,0x6e9c42,0x848484,
    0x87b754,0x9d9d9d,0x9ed065,0xb5b5b5,0xb4e775,0xcbcbcb,0xc8fc84,0xdfdfdf,
    0x303800,0x2f2f2f,0x505916,0x4f4f4f,0x6d762b,0x6b6b6b,0x88923e,0x858585,
    0xa0ab4f,0x9d9d9d,0xb7c25f,0xb3b3b3,0xccd86e,0xc8c8c8,0xe0ec7c,0xdcdcdc,
    0x482c00,0x2f2f2f,0x694d14,0x4f4f4f,0x866a26,0x6b6b6b,0xa28638,0x858585,
    0xbb9f47,0x9d9d9d,0xd2b656,0xb3b3b3,0xe8cc63,0xc8c8c8,0xfce070,0xdcdcdc
};

static uint32_t PALPalette[256] = {
    0x000000,0x000000,0x2b2b2b,0x2b2b2b,0x525252,0x525252,0x767676,0x767676,
    0x979797,0x979797,0xb6b6b6,0xb6b6b6,0xd2d2d2,0xd2d2d2,0xececec,0xececec,
    0x000000,0x000000,0x2b2b2b,0x2b2b2b,0x525252,0x525252,0x767676,0x767676,
    0x979797,0x979797,0xb6b6b6,0xb6b6b6,0xd2d2d2,0xd2d2d2,0xececec,0xececec,
    0x805800,0x5a5a5a,0x96711a,0x727272,0xab8732,0x888888,0xbe9c48,0x9d9d9d,
    0xcfaf5c,0xafafaf,0xdfc06f,0xc0c0c0,0xeed180,0xd0d0d0,0xfce090,0xdfdfdf,
    0x445c00,0x4a4a4a,0x5e791a,0x666666,0x769332,0x7f7f7f,0x8cac48,0x979797,
    0xa0c25c,0xacacac,0xb3d76f,0xc0c0c0,0xc4ea80,0xd3d3d3,0xd4fc90,0xe4e4e4,
    0x703400,0x404040,0x89511a,0x5b5b5b,0xa06b32,0x747474,0xb68448,0x8c8c8c,
    0xc99a5c,0xa1a1a1,0xdcaf6f,0xb5b5b5,0xecc280,0xc7c7c7,0xfcd490,0xd8d8d8,
    0x006414,0x3d3d3d,0x1a8035,0x595959,0x329852,0x727272,0x48b06e,0x898989,
    0x5cc587,0x9f9f9f,0x6fd99e,0xb3b3b3,0x80ebb4,0xc5c5c5,0x90fcc8,0xd6d6d6,
    0x700014,0x242424,0x891a35,0x3e3e3e,0xa03252,0x575757,0xb6486e,0x6d6d6d,
    0xc95c87,0x818181,0xdc6f9e,0x959595,0xec80b4,0xa6a6a6,0xfc90c8,0xb7b7b7,
    0x005c5c,0x404040,0x1a7676,0x5a5a5a,0x328e8e,0x727272,0x48a4a4,0x888888,
    0x5cb8b8,0x9c9c9c,0x6fcbcb,0xafafaf,0x80dcdc,0xc0c0c0,0x90ecec,0xd0d0d0,
    0x70005c,0x2c2c2c,0x841a74,0x444444,0x963289,0x5a5a5a,0xa8489e,0x6e6e6e,
    0xb75cb0,0x818181,0xc66fc1,0x929292,0xd380d1,0xa2a2a2,0xe090e0,0xb1b1b1,
    0x003c70,0x303030,0x195a89,0x4c4c4c,0x2f75a0,0x656565,0x448eb6,0x7c7c7c,
    0x57a5c9,0x929292,0x68badc,0xa5a5a5,0x79ceec,0xb8b8b8,0x88e0fc,0xc9c9c9,
    0x580070,0x272727,0x6e1a89,0x404040,0x8332a0,0x575757,0x9648b6,0x6c6c6c,
    0xa75cc9,0x7f7f7f,0xb76fdc,0x919191,0xc680ec,0xa1a1a1,0xd490fc,0xb1b1b1,
    0x002070,0x202020,0x193f89,0x3c3c3c,0x2f5aa0,0x555555,0x4474b6,0x6d6d6d,
    0x578bc9,0x838383,0x68a1dc,0x979797,0x79b5ec,0xa9a9a9,0x88c8fc,0xbbbbbb,
    0x340080,0x1e1e1e,0x4a1a96,0x363636,0x5f32ab,0x4d4d4d,0x7248be,0x626262,
    0x835ccf,0x757575,0x936fdf,0x878787,0xa280ee,0x979797,0xb090fc,0xa6a6a6,
    0x000088,0x101010,0x1a1a9d,0x292929,0x3232b0,0x404040,0x4848c2,0x565656,
    0x5c5cd2,0x696969,0x6f6fe1,0x7c7c7c,0x8080ef,0x8d8d8d,0x9090fc,0x9c9c9c,
    0x000000,0x000000,0x2b2b2b,0x2b2b2b,0x525252,0x525252,0x767676,0x767676,
    0x979797,0x979797,0xb6b6b6,0xb6b6b6,0xd2d2d2,0xd2d2d2,0xececec,0xececec,
    0x000000,0x000000,0x2b2b2b,0x2b2b2b,0x525252,0x525252,0x767676,0x767676,
    0x979797,0x979797,0xb6b6b6,0xb6b6b6,0xd2d2d2,0xd2d2d2,0xececec,0xececec
};
static void printRam(const void* data, size_t size){
	system("clear");
	uint8_t* nData = (uint8_t*)data;
   cout << "------------------------------------------------------" << endl;
   cout << "      ";
   for (int i=0; i<16; i++){
	   cout << std::setw(1) << std::hex << i << "  ";
   }
   for (uint8_t i = 0; i < size; i++){
//	   if (i % 100 == 0)
//		   cout << "------------####--------------- I IS HUGE----------------###----------------"<<endl;
//	   cerr << +i << endl;
	   if(i%16 == 0){
		   cout << endl << setw(4) << std::hex << std::setfill('0') << +i << " ";
	   }
	   cout << std::hex << std::setw(2) << std::setfill('0')<<  +nData[i] << " ";
   }
   cout << endl;
}
static void getRGB(int val, int &r, int &g, int &b) {

	r = (NTSCPalette[val] >> 16) & 0xFF;
    g = (NTSCPalette[val] >>  8) & 0xFF;
    b = (NTSCPalette[val] >>  0) & 0xFF;
//	r = (PALPalette[val] >> 16) & 0xFF;
//    g = (PALPalette[val] >>  8) & 0xFF;
//    b = (PALPalette[val] >>  0) & 0xFF;
}

#ifdef __USE_SDL
DisplayScreen::DisplayScreen(RetroAgent& ragent
//		MediaSource* mediaSource,
//                             Sound* sound
                      //    ColourPalette &palette
                             ):
        manual_control_active(false),
//        media_source(mediaSource),
//        my_sound(sound),
 //     colour_palette(),
        delay_msec(17)
{
	//colour_palette.setPalette("standard","NTSC");
    screen_height = ragent.getHeight();
    screen_width = ragent.getWidth(); //screen does not affect the colors only the number of images per frame, higher width = more images per frame
//    screen_height = media_source->height();
//    screen_width = media_source->width();
//    assert(window_height >= screen_height);
//    assert(window_width >= screen_width);
    yratio = window_height / (float) screen_height;
    xratio = window_width / (float) screen_width;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
   screen = SDL_SetVideoMode(window_width, window_height, 8, SDL_HWPALETTE);
//    screen = SDL_SetVideoMode(window_width, window_height, 32, SDL_HWPALETTE);
//screen->pitch =screen->pitch/4; // the pitch is responsible for the overlap is pitch=pitch/2 then image will appear twice probably not the proiblem

    if (screen == NULL) {
        fprintf(stderr, "Couldn't Initialize Screen: %s\n", SDL_GetError());
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
//

void DisplayScreen::display_screen(RetroAgent& ragent) {
//	ragent.updateScreen();

    if (SDL_MUSTLOCK(screen)) {
      if (SDL_LockSurface(screen) < 0 ) {
        fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
        return;
      }
    }

    // Convert the media sources frame into the screen matrix representation
    int xciel = int(xratio) + 1;
    int yciel = int(yratio) + 1;
//    uInt8* pi_curr_frame_buffer = media_source->currentFrameBuffer(); // currentBuffer is pure virtual in media_source
    uint8_t* pi_curr_frame_buffer = ragent.getCurrentBuffer();
    // Shai: temp fix
    // Shai: temp fix
    //   printRam(pi_curr_frame_buffer,screen_width * screen_height);
//     cout<< "in Display and size of current frame is is: " << sizeof(pi_curr_frame_buffer) <<endl;
    int y, x, r, g, b;
    uint8_t temp;
    SDL_Rect rect;
  //  for (int i = 0; i < screen_width * screen_height*4; i=i+4) {
  for (int i = 0; i < screen_width * screen_height; i++) {
        y = i / screen_width;
        x = i - (y * screen_width);
       // cout<< "in Display and i is: " << i <<endl;
   //     temp=pi_curr_frame_buffer[i];
     //    cout<< "in Display and temp is: " << temp <<endl;

         getRGB(pi_curr_frame_buffer[i], r, g, b);
   //     getRGB(temp, r, g, b);
        rect.x = (int)(x * xratio);
        rect.y = (int)(y * yratio);
        rect.w = xciel;
        rect.h = yciel;
        SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, r, g, b));
    }
    if (SDL_MUSTLOCK(screen)) {
      SDL_UnlockSurface(screen);
    }
    SDL_UpdateRect(screen, 0, 0, 0, 0);
    poll();

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
                                "Arrow keys [Fire] Space [NO-OP] Return.\n");
                    } else {
                        fprintf(stderr, "Manual Control Disabled\n");
                    }
                    break;
                case SDLK_s:
                    delay_msec = delay_msec > 5 ? delay_msec - 5 : 0;
                    fprintf(stderr, "[Speedup] %d msec delay\n", delay_msec);
                    break;
                case SDLK_a:
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

Action DisplayScreen::getUserAction() { //shai: currently remains commented out as ale_controller is not used, if added (after consult with mark on why screen needs action) than needs be be expand
//    if (!manual_control_active) {
//        return UNDEFINED;
//    }
//    Action a = PLAYER_A_NOOP;
//    poll();
//    SDL_PumpEvents();
//    Uint8* keymap = SDL_GetKeyState(NULL);
//    // Break out of this loop if the 'p' key is pressed
//    if (keymap[SDLK_p]) {
//      return PLAYER_A_NOOP;
//      // Triple Actions
//    } else if (keymap[SDLK_UP] && keymap[SDLK_RIGHT] && keymap[SDLK_SPACE]) {
//      a = PLAYER_A_UPRIGHTFIRE;
//    } else if (keymap[SDLK_UP] && keymap[SDLK_LEFT] && keymap[SDLK_SPACE]) {
//      a = PLAYER_A_UPLEFTFIRE;
//    } else if (keymap[SDLK_DOWN] && keymap[SDLK_RIGHT] && keymap[SDLK_SPACE]) {
//      a = PLAYER_A_DOWNRIGHTFIRE;
//    } else if (keymap[SDLK_DOWN] && keymap[SDLK_LEFT] && keymap[SDLK_SPACE]) {
//      a = PLAYER_A_DOWNLEFTFIRE;
//      // Double Actions
//    } else if (keymap[SDLK_UP] && keymap[SDLK_LEFT]) {
//      a = PLAYER_A_UPLEFT;
//    } else if (keymap[SDLK_UP] && keymap[SDLK_RIGHT]) {
//      a = PLAYER_A_UPRIGHT;
//    } else if (keymap[SDLK_DOWN] && keymap[SDLK_LEFT]) {
//      a = PLAYER_A_DOWNLEFT;
//    } else if (keymap[SDLK_DOWN] && keymap[SDLK_RIGHT]) {
//      a = PLAYER_A_DOWNRIGHT;
//    } else if (keymap[SDLK_UP] && keymap[SDLK_SPACE]) {
//      a = PLAYER_A_UPFIRE;
//    } else if (keymap[SDLK_DOWN] && keymap[SDLK_SPACE]) {
//      a = PLAYER_A_DOWNFIRE;
//    } else if (keymap[SDLK_LEFT] && keymap[SDLK_SPACE]) {
//      a = PLAYER_A_LEFTFIRE;
//    } else if (keymap[SDLK_RIGHT] && keymap[SDLK_SPACE]) {
//      a = PLAYER_A_RIGHTFIRE;
//      // Single Actions
//    } else if (keymap[SDLK_SPACE]) {
//      a = PLAYER_A_FIRE;
//    } else if (keymap[SDLK_RETURN]) {
//      a = PLAYER_A_NOOP;
//    } else if (keymap[SDLK_LEFT]) {
//      a = PLAYER_A_LEFT;
//    } else if (keymap[SDLK_RIGHT]) {
//      a = PLAYER_A_RIGHT;
//    } else if (keymap[SDLK_UP]) {
//      a = PLAYER_A_UP;
//    } else if (keymap[SDLK_DOWN]) {
//      a = PLAYER_A_DOWN;
//    }
//    return a;
}

#endif // __USE_SDL
