#ifndef __SUPER_DOUBLE_DRAGON_SETTINGS_HPP__
#define __SUPER_DOUBLE_DRAGON_HPP__
/* RL wrapper for AtariCollection settings */

#include "../SnesSettings.hpp"

namespace rle {

struct SuperDoubleDragonSettings : public SnesSettings {

    public:

        SuperDoubleDragonSettings();

        // reset
        virtual void reset();

        // the rom-name
        virtual const char* rom() const { return "super_double_dragon"; }

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
  int m_prevLife = 0;  //previous position
  int m_prevEnemyKills = 0;
};

} // namespace rle

#endif // __SUPER_DOUBLE_DRAGON__
