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

class RleSystem{
public:
    /**
      Create a new RleSystem abstract class
    */
    RleSystem(pRetroAgent retroagent, pSettings settings);

    /**
      Destructor
    */
    virtual ~RleSystem();

    RleSystem(const RleSystem&) = delete;

    RleSystem& operator = (const RleSystem&) = delete;

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
      Get the settings object of the system

      @return The settings object
    */
    pSettings settings() const { return mySettings; }

    /**
      Get the RetroAgent object of the system

      @return The RetorAgent object
    */
    pRetroAgent getRetroAgent() const { return myRetroAgent; }

    /**
      Perform one step in the emulator
     */
    void step();

    /**
      Set all config file paths for the RleSystem.
    */
    void setConfigPaths();

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
    
protected:

    /**
      Set the locations of config file
    */
    void setConfigFile(const std::string& file) { myConfigFile = file; }
    
    // Pointer to the Sound object
//    Sound* mySound;

    // Pointer to the Settings object
    pSettings mySettings;

    // Pointer to RetroAgent
    pRetroAgent myRetroAgent;	// RLE

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
    std::string myRomFile;
    std::string myCoreFile;
    std::string myConfigFile;
    
public: //ALE 
    shared_ptr<DisplayScreen> p_display_screen; //MHAUSKN

};

typedef shared_ptr<RleSystem> pRleSystem;

} // namespace rle

#endif
