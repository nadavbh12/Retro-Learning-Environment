//============================================================================
//
//   SSSS    tt          lll  lll       
//  SS  SS   tt           ll   ll        
//  SS     tttttt  eeee   ll   ll   aaaa 
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
//
// See the file "license" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: RleSystem.hxx,v 1.55 2007/08/12 23:05:12 stephena Exp $
//============================================================================

#ifndef RleSystem_HXX
#define RleSystem_HXX

namespace rle {

//class PropertiesSet;
class GameController;
class Menu;
class CommandMenu;
class Launcher;
class Debugger;
class CheatManager;
class VideoDialog;

}

#include "../common/Array.hxx"
#include "Settings.hxx"
#include "../common/display_screen.h" 
#include "../common/ScreenExporter.hpp"
#include "../common/Log.hpp"
#include "Random.hxx"
#include "../common/bspf.hxx"

#include "RetroAgent.h"


namespace rle {

struct Resolution {
  uInt32 width;
  uInt32 height;
  std::string name;
};
typedef Common::Array<Resolution> ResolutionList;

class RleSystem
{
  public:
    /**
      Create a new RleSystem abstract class
    */
    RleSystem(RetroAgent* retroagent);

    /**
      Destructor
    */
    virtual ~RleSystem();

    RleSystem(const RleSystem&) = delete;

    RleSystem& operator = (const RleSystem&) = delete;

    /**
      Create all child objects which belong to this RleSystem
    */
    virtual bool create();

    /**
      Adds the specified settings object to the system.

      @param settings The settings object to add 
    */
    void attach(Settings* settings) { mySettings = settings; }

    /**
      Get the frame buffer of the system

      @return The frame buffer
    */
    uint8_t* getCurrentFrameBuffer() const { return (uint8_t*)myRetroAgent->getCurrentBuffer();}

    /**
      Returns the size of the buffer in bytes;
    */
    int getBufferSize() const { return myRetroAgent->getBufferSize();}
    /**
      Get the sound object of the system

      @return The sound object
    */
//    inline Sound& sound() const { return *mySound; }

    /**
      Get the settings object of the system

      @return The settings object
    */
    inline Settings& settings() const { return *mySettings; }

    /**
      Get the RetroAgent object of the system

      @return The RetorAgent object
    */
    inline RetroAgent& getRetroAgent() const { return *myRetroAgent; }

    /**
      Perform one step in the emulator
     */
    void step();

    /**
      Set the framerate for the video system.  It's placed in this class since
      the mainLoop() method is defined here.

      @param framerate  The video framerate to use
    */
    virtual void setFramerate(uInt32 framerate);

    /**
      Set all config file paths for the RleSystem.
    */
    void setConfigPaths();

    /**
      Get the current framerate for the video system.

      @return  The video framerate currently in use
    */
    inline uInt32 frameRate() const { return myDisplayFrameRate; }

    /**
      Get the maximum dimensions of a window for the video hardware.
    */
    const uInt32 desktopWidth() const  { return myDesktopWidth; }
    const uInt32 desktopHeight() const { return myDesktopHeight; }

    /**
      Get the supported fullscreen resolutions for the video hardware.

      @return  An array of supported resolutions
    */
    const ResolutionList& supportedResolutions() const { return myResolutions; }

    /**
      Return the default directory for storing data.
    */
    const std::string& baseDir() const { return myBaseDir; }

    /**
      This method should be called to get the full path of the config file.

      @return String representing the full path of the config filename.
    */
    const std::string& configFile() const { return myConfigFile; }

    /**
      This method should be called to get the full path of the currently
      loaded ROM.

      @return String representing the full path of the ROM file.
    */
    const std::string& romFile() const { return myRomFile; }

    /**
      This method should be called to get the full path of the currently
      loaded core.

      @return String representing the full path of the core file.
    */
    const std::string& coreFile() const { return myCoreFile; }

    /**
      Switches between software and OpenGL framebuffer modes.
    */
    // TODO SN: maybe implement the options when to select how to initialize SDL in display_screen
    //ALE  void toggleFrameBuffer();

    /**
	  Loads the emulator core.
	*/
	bool loadCore(const string& core);
    /**
	  Loads the rom file.
	  Must be called after openCore
	*/
    bool loadRom(const string& rom);

    /**
      Returns the random number generator for this emulator.
    */
    Random& rng() { return myRandGen; }

    /**
      Resets the seed for our random number generator.
    */
    void resetRNGSeed();

    /** 
      Serializes the RleSystem state.
    */
    bool saveState(Serializer& out);

    /** 
      Deserializes the RleSystem state.
    */
    bool loadState(Deserializer& in);

  public:
    //////////////////////////////////////////////////////////////////////
    // The following methods are system-specific and must be implemented
    // in derived classes.
    //////////////////////////////////////////////////////////////////////
    /**
      This method returns number of ticks in microseconds.

      @return Current time in microseconds.
    */
    virtual uInt32 getTicks() = 0;
    
  protected:
    /**
      Set the base directory for all Stella files (these files may be
      located in other places through settings).
    */
    void setBaseDir(const std::string& basedir);

    /**
      Set the locations of config file
    */
    void setConfigFile(const std::string& file) { myConfigFile = file; }
    
    // Pointer to the Sound object
//    Sound* mySound;

    // Pointer to the Settings object
    Settings* mySettings;

    // Pointer to RetroAgent
    RetroAgent* myRetroAgent;	// RLE

    // Random number generator shared across the emulator's components
    Random myRandGen; 

    // Maximum dimensions of the desktop area
    uInt32 myDesktopWidth, myDesktopHeight;

    // Supported fullscreen resolutions
    ResolutionList myResolutions;

    // Number of times per second to iterate through the main loop
    uInt32 myDisplayFrameRate;

    // Indicates that the emulation should not occur on the next time step
    // This is reset to false after one step
    bool mySkipEmulation;

  private:
    std::string myBaseDir;
    std::string myRomFile;
    std::string myCoreFile;
    std::string myConfigFile;
    
    public: //ALE 
    // Time per frame for a video update, based on the current framerate
    uInt32 myTimePerFrame;

    // Indicates whether the main processing loop should proceed
    struct TimingInfo {
      uInt32 start;
      uInt32 current;
      uInt32 virt;
      uInt32 totalTime;
      uInt32 totalFrames;
    };
    TimingInfo myTimingInfo;


    // Table of RGB values for GUI elements
    //ALE  static uInt32 ourGUIColors[kNumUIPrlettes][kNumColors-256];
  public:
    DisplayScreen* p_display_screen; //MHAUSKN
  
  private:

    /**
      Creates the various sound devices available in this system
      (for now, that means either 'SDL' or 'Null').
    */
//    void createSound();

    /**
      Query valid info for creating a valid console.

      @return Success or failure for a valid console
    */
//    bool queryConsoleInfo(const uInt8* image, uInt32 size, const std::string& md5,
//                          Cartridge** cart, Properties& props);

    /**
      Initializes the timing so that the mainloop is reset to its
      initial values.
    */
    void resetLoopTiming();

};

} // namespace rle

#endif
