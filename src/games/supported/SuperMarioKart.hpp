#ifndef __SUPER_MARIO_KART_SETTINGS_HPP__
#define __SUPER_MARIO_KART_SETTINGS_HPP__
/* RL wrapper for AtariCollection settings */

#include "../SnesSettings.hpp"

namespace rle {

struct SuperMarioKartSettings : public SnesSettings {

    public:

        SuperMarioKartSettings();

        // reset
        virtual void reset();

        // the rom-name
        virtual const char* rom() const { return "super_mario_kart"; }

        // create a new instance of the rom
        virtual RomSettings* clone() const;

        // process the latest information from ALE
        virtual void step(const RleSystem& system);

        // saves the state of the rom settings
        virtual void saveState( Serializer & ser );

        // loads the state of the rom settings
        virtual void loadState( Deserializer & des );

        virtual ActionVect selectChar(int character_index);

        virtual int getCharacterIndex(const RleSystem& system);
  
        virtual const int lives() { return 0; }

        virtual ActionVect getStartingActions(const RleSystem& system);

protected:
  int m_lives = 4;
  int m_prevPosition = 0;  //previous position
  int m_current_character = 0;
};

} // namespace rle

#endif // __SUPER_MARIO_KART_SETTINGS_HPP__
