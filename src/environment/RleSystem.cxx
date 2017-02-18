
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

//#include "bspf.hxx"

using namespace rle;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
RleSystem::RleSystem(pRetroAgent retroagent, pSettings settings)
  : 
    mySettings(settings),	//RLE
	myRetroAgent(retroagent),	//RLE
    myRomFile(""),
	myCoreFile("")
//    p_display_screen(NULL)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
RleSystem::~RleSystem(){}

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

void RleSystem::step() {
	myRetroAgent->run();
}

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
