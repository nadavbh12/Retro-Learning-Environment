#ifndef __CONTRA_III_SETTINGS_HPP__
#define __CONTRA_III_SETTINGS_HPP__
/* RL wrapper for AtariCollection settings */

#include "../SnesSettings.hpp"

namespace rle {

struct ContraIIISettings : public SnesSettings {

    public:

        ContraIIISettings();

        // reset
        virtual void reset();

        // the rom-name
        virtual const char* rom() const { return "contra_iii"; }

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
  int m_prevScore = 0;
};

} // namespace rle

#endif // __SUPER_MARIO_KART_SETTINGS_HPP__
