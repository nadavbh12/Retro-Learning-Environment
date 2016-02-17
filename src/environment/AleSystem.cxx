
#include <cassert>
#include <sstream>
#include <fstream>
#include <iostream>
#include <zlib.h>
#include <string.h>
using namespace std;

#include "FSNode.hxx"
#include "Settings.hxx"
#include "AleSystem.hxx"
#include "SoundSDL.hxx"
//#define MAX_ROM_SIZE  512 * 1024

#include <time.h>

#include "bspf.hxx"

using namespace ale;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
AleSystem::AleSystem(RetroAgent* retroagent)
  : 
	myRetroAgent(retroagent),	//SLE
//    mySound(NULL),
    mySettings(NULL),
    myQuitLoop(false),
    mySkipEmulation(false),
    myRomFile(""),
	myCoreFile(""),
    myFeatures(""),
    p_display_screen(NULL)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
AleSystem::~AleSystem()
{

  // AleSystem takes responsibility for framebuffer and sound,
  // since it created them
//  if (mySound != NULL)
//    delete mySound;

  if (p_display_screen != NULL) {
      delete p_display_screen;
  }
    
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool AleSystem::create()
{
  // Create the sound object; the sound subsystem isn't actually
  // opened until needed, so this is non-blocking (on those systems
  // that only have a single sound device (no hardware mixing)
  //  createSound();
  
  // Seed RNG. This will likely get re-called, e.g. by the ALEInterface, but is needed
  // by other interfaces.
  resetRNGSeed();

  return true;
}

void AleSystem::resetRNGSeed() {

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

//bool AleSystem::saveState(Serializer& out) {
//
//    // Here we serialize the RNG state.
//    return myRandGen.saveState(out);
//}
//
//bool AleSystem::loadState(Deserializer& in) {
//    return myRandGen.loadState(in);
//}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//void AleSystem::setConfigPaths()
//{
//  myGameListCacheFile = myBaseDir + BSPF_PATH_SEPARATOR + "stella.cache";
//
//  myCheatFile = mySettings->getString("cheatfile");
//  if(myCheatFile == "")
//    myCheatFile = myBaseDir + BSPF_PATH_SEPARATOR + "stella.cht";
//  mySettings->setString("cheatfile", myCheatFile);
//
//  myPaletteFile = mySettings->getString("palettefile");
//  if(myPaletteFile == "")
//    myPaletteFile = myBaseDir + BSPF_PATH_SEPARATOR + "stella.pal";
//  mySettings->setString("palettefile", myPaletteFile);
//
//  myPropertiesFile = mySettings->getString("propsfile");
//  if(myPropertiesFile == "")
//    myPropertiesFile = myBaseDir + BSPF_PATH_SEPARATOR + "stella.pro";
//  mySettings->setString("propsfile", myPropertiesFile);
//}

void AleSystem::setBaseDir(const string& basedir)
{
  myBaseDir = basedir;
  if(!FilesystemNode::dirExists(myBaseDir))
    FilesystemNode::makeDir(myBaseDir);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void AleSystem::setFramerate(uInt32 framerate)
{
  myDisplayFrameRate = framerate;
  myTimePerFrame = (uInt32)(1000000.0 / (double)myDisplayFrameRate);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//void AleSystem::createSound()
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
bool AleSystem::loadCore(const string& corePath){
	myCoreFile = corePath;
	myRetroAgent->loadCore(corePath);
	return true;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool AleSystem::loadRom(const string& rom){
	myRomFile = rom;
	myRetroAgent->loadRom(rom);
	return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//void AleSystem::resetLoopTiming()
//{
//  memset(&myTimingInfo, 0, sizeof(TimingInfo));
//  myTimingInfo.start = getTicks();
//  myTimingInfo.virt = getTicks();
//}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
AleSystem::AleSystem(const AleSystem& alesystem) {
	myRetroAgent = alesystem.myRetroAgent;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
AleSystem& AleSystem::operator = (const AleSystem&)
{
  assert(false);

  return *this;
}
