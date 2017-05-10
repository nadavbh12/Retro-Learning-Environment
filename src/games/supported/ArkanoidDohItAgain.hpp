#ifndef __ARKANOID_DOH_IT_AGAIN_SETTINGS_HPP__
#define __ARKANOID_DOH_IT_AGAIN_SETTINGS_HPP__
/* RL wrapper for AtariCollection settings */

#include "../SnesSettings.hpp"

namespace rle {

struct ArkanoidDohItAgainSettings : public SnesSettings {

    public:

        ArkanoidDohItAgainSettings();

        // reset
        virtual void reset();

        // the rom-name
        virtual const char* rom() const { return "arkanoid_doh_it_again"; }

        // create a new instance of the rom
        virtual RomSettings* clone() const;

        // process the latest information from ALE
        virtual void step(const RleSystem& system);

        // saves the state of the rom settings
        virtual void saveState( Serializer & ser );

        // loads the state of the rom settings
        virtual void loadState( Deserializer & des );

        virtual const int lives() { return 0; }

        virtual ActionVect getStartingActions(const RleSystem& system);

protected:
  int m_lives = 3;
  int m_prevPosition = 0;  //previous position
};

} // namespace rle

#endif // __ARKANOID_DOH_IT_AGAIN_SETTINGS_HPP__
