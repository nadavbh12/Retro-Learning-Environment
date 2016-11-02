
#include <cassert>
#include <sstream>
#include <fstream>
#include <iostream>
#include <zlib.h>
#include <string.h>
using namespace std;

#include "FSNode.hxx"
#include "Settings.hxx"
#include "RleSystem.hxx"
#include <time.h>

#include "bspf.hxx"

using namespace rle;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
RleSystem::RleSystem(RetroAgent* retroagent)
  : 
    mySettings(NULL),
	myRetroAgent(retroagent),	//RLE
    myRomFile(""),
	myCoreFile(""),
    p_display_screen(NULL)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
RleSystem::~RleSystem()
{

  // RleSystem takes responsibility for framebuffer and sound,
  // since it created them
//  if (mySound != NULL)
//    delete mySound;

  if (p_display_screen != NULL) {
      delete p_display_screen;
  }
    
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool RleSystem::create()
{
  // Create the sound object; the sound subsystem isn't actually
  // opened until needed, so this is non-blocking (on those systems
  // that only have a single sound device (no hardware mixing)
  //  createSound();
  
  // Seed RNG. This will likely get re-called, e.g. by the RLEInterface, but is needed
  // by other interfaces.
  resetRNGSeed();

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void RleSystem::resetRNGSeed() {

  // We seed the random number generator. The 'time' seed is somewhat redundant, since the
  // rng defaults to time. But we'll do it anyway.
  if (mySettings->getInt("random_seed") == 0) {
    myRandGen.seed((uInt32)time(NULL));
  } else {
    int seed = mySettings->getInt("random_seed");
    assert(seed >= 0);
    myRandGen.seed((uInt32)seed);
  }
}

bool RleSystem::saveState(Serializer& out) {

//     Here we serialize the RNG state.
	    return myRandGen.saveState(out);
}

bool RleSystem::loadState(Deserializer& in) {
    return myRandGen.loadState(in);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//void RleSystem::setConfigPaths()
//{
//  myGameListCacheFile = myBaseDir + BSPF_PATH_SEPARATOR + "stella.cache";
//
//  myCheatFile = mySettings->getString("cheatfile");
//  if(myCheatFile == "")
//    myCheatFile = myBaseDir + BSPF_PATH_SEPARATOR + "stella.cht";
//  mySettings->setString("cheatfile", myCheatFile);
//
//  myPrletteFile = mySettings->getString("prlettefile");
//  if(myPrletteFile == "")
//    myPrletteFile = myBaseDir + BSPF_PATH_SEPARATOR + "stella.pal";
//  mySettings->setString("prlettefile", myPrletteFile);
//
//  myPropertiesFile = mySettings->getString("propsfile");
//  if(myPropertiesFile == "")
//    myPropertiesFile = myBaseDir + BSPF_PATH_SEPARATOR + "stella.pro";
//  mySettings->setString("propsfile", myPropertiesFile);
//}

void RleSystem::setBaseDir(const string& basedir)
{
  myBaseDir = basedir;
  if(!FilesystemNode::dirExists(myBaseDir))
    FilesystemNode::makeDir(myBaseDir);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void RleSystem::step() {
	myRetroAgent->run();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void RleSystem::setFramerate(uInt32 framerate)
{
  myDisplayFrameRate = framerate;
  myTimePerFrame = (uInt32)(1000000.0 / (double)myDisplayFrameRate);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//void RleSystem::createSound()
//{
//  if (mySound != NULL) {
//    delete mySound;
//  }
//  mySound = NULL;
//
//#ifdef SOUND_SUPPORT
//  // If requested (& supported), enable sound
//  if (mySettings->getBool("sound") == true) {
//      mySound = new SoundSDL(this);
//      mySound->initialize();
//  }
//  else {
//      mySound = new SoundNull(this);
//  }
//#else
//  mySettings->setBool("sound", false);
//  mySound = new SoundNull(this);
//#endif
//}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool RleSystem::loadCore(const string& corePath){
	myCoreFile = corePath;
	myRetroAgent->loadCore(corePath);
	return true;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool RleSystem::loadRom(const string& rom){
	myRomFile = rom;
	myRetroAgent->loadRom(rom);
	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//void RleSystem::resetLoopTiming()
//{
//  memset(&myTimingInfo, 0, sizeof(TimingInfo));
//  myTimingInfo.start = getTicks();
//  myTimingInfo.virt = getTicks();
//}
