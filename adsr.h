//----------------------------------//
// ADSR class for Arduino
// by mo-thunderz
// version 1.2
// last update: 14.08.2022
//----------------------------------//

#include "Arduino.h"

#ifndef ADSR
#define ADSR

// for array for lookup table
#define ARRAY_SIZE 1024                     // number of time points
#define ATTACK_ALPHA 0.995                  // varies between 0.9 (steep curve) and 0.9995 (straight line)
#define ATTACK_DECAY_RELEASE 0.997          // fits to ARRAY_SIZE 1024


// Midi trigger -> on/off
class adsr
{
    public:
        // constructor
        adsr(int dacSize);

        void setAttack(unsigned long l_attack);
        void setDecay(unsigned long l_decay);
        void setSustain(int l_sustain);
        void setRelease(unsigned long l_release);
        void noteOn(unsigned long l_micros);
        void noteOff(unsigned long l_micros);
        int getWave(unsigned long l_micros);
        void setResetAttack(bool l_reset_attack);

    private:
        int _attack_table[ARRAY_SIZE];
        int _decay_release_table[ARRAY_SIZE];

        int _vertical_resolution;                               // number of bits for output, control, etc
        unsigned long _attack = 0;                              // 0 to 20 sec
        unsigned long _decay = 0;                               // 1ms to 60 sec
        int _sustain = 0;                                       // 0 to -60dB -> then -inf
        unsigned long _release = 0;                             // 1ms to 60 sec
        bool _reset_attack = false;                             // if _reset_attack is "true" a new trigger starts with 0, if _reset_attack is false it starts with the current output value

        // time stamp for note on and note off
        unsigned long _t_note_on = 0;
        unsigned long _t_note_off = 0;

        // internal values needed to transition to new pulse (attack) and to release at any point in time
        int _adsr_output;
        int _release_start;
        int _attack_start;
        int _notes_pressed = 0;
};

#endif
