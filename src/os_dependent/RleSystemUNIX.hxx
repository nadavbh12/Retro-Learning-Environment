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
// $Id: RleSystemUNIX.hxx,v 1.16 2007/07/19 16:21:39 stephena Exp $
//============================================================================

#ifndef OSYSTEM_UNIX_HXX
#define OSYSTEM_UNIX_HXX

#include "../common/bspf.hxx"
#include "../environment/RleSystem.hxx"

namespace rle {

class RetroAgent;

/**
  This class defines UNIX-like OS's (Linux) system specific settings.

  @author  Stephen Anthony
  @version $Id: RleSystemUNIX.hxx,v 1.16 2007/07/19 16:21:39 stephena Exp $
*/
class RleSystemUNIX : public RleSystem
{
  public:
    /**
      Create a new UNIX-specific operating system object
    */
    RleSystemUNIX(RetroAgent* retroagent);

    /**
      Destructor
    */
    virtual ~RleSystemUNIX();

    /**
      This method returns number of ticks in microseconds.

      @return Current time in microseconds.
    */
    uInt32 getTicks();
};

} // namespace rle

#endif
