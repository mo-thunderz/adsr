//----------------------------------//
// ADSR class for Arduino
// by mo-thunderz
// version 1.2
// last update: 14.08.2020
//----------------------------------//

#include "Arduino.h"
#include "adsr.h"
#include <math.h>

adsr::adsr(int l_vertical_resolution)
{
    _vertical_resolution = l_vertical_resolution;                               // store vertical resolution (DAC_Size)
    _attack = 100000;                                                           // take 100ms as initial value for Attack
    _sustain = l_vertical_resolution/2;                                         // take half the DAC_size as initial value for sustain
    _decay = 100000;                                                            // take 100ms as initial value for Decay
    _release = 100000;                                                          // take 100ms as initial value for Release

    for (int i = 0; i<ARRAY_SIZE; i++) {                                        // Create look-up table for Attack
        _attack_table[i] = i;
        _decay_release_table[i] = _vertical_resolution - 1 - i;
    }

    for (int i = 0; i<ARRAY_SIZE-1; i++) {                                      // Create look-up table for Decay
        _attack_table[i+1] = (1.0 - ATTACK_ALPHA) * (_vertical_resolution - 1) + ATTACK_ALPHA * _attack_table[i];
        _decay_release_table[i+1] = ATTACK_DECAY_RELEASE * _decay_release_table[i];
    }

    for (int i = 0; i<ARRAY_SIZE; i++) {                                        // normalize table to min and max
        _attack_table[i] = map(_attack_table[i], 0, _attack_table[ARRAY_SIZE-1], 0, _vertical_resolution - 1);
        _decay_release_table[i] = map(_decay_release_table[i], _decay_release_table[ARRAY_SIZE-1], _decay_release_table[0], 0, _vertical_resolution - 1);
    }
}

void adsr::setResetAttack(bool l_reset_attack)
{
    _reset_attack = l_reset_attack;
}

void adsr::setAttack(unsigned long l_attack)
{
    _attack = l_attack;
}

void adsr::setDecay(unsigned long l_decay)
{
    _decay = l_decay;
}

void adsr::setSustain(int l_sustain)
{
    if(l_sustain < 0)
        l_sustain = 0;
    if(l_sustain >= _vertical_resolution)
        l_sustain = _vertical_resolution - 1;
    _sustain = l_sustain;
}

void adsr::setRelease(unsigned long l_release)
{
    _release = l_release;
}

void adsr::noteOn(unsigned long l_micros) {
    _t_note_on = l_micros;                          // set new timestamp for note_on
    if(_reset_attack)                               // set start value new Attack
        _attack_start = 0;							// if _reset_attack equals true, a new trigger starts with 0
    else
        _attack_start = _adsr_output;               // if _reset_attack equals false, a new trigger starts with the current value
    _notes_pressed++;                               // increase number of pressed notes with one
}

void adsr::noteOff(unsigned long l_micros) {
    _notes_pressed--;
    if(_notes_pressed <= 0) {                       // if all notes are depressed - start release
        _t_note_off = l_micros;                     // set timestamp for note off
        _release_start = _adsr_output;              // set start value for release
        _notes_pressed = 0;
    }
}

int adsr::getWave(unsigned long l_micros)
{
    unsigned long delta = 0;
    if(_t_note_off < _t_note_on)        {           // if note is pressed
        delta = l_micros - _t_note_on;
        if(delta < _attack)                         // Attack
            _adsr_output = map(_attack_table[(int)floor(ARRAY_SIZE * (float) delta / (float)_attack)], 0, _vertical_resolution - 1, _attack_start, _vertical_resolution - 1);       //
        else if(delta < _attack + _decay) {         // Decay
            delta = l_micros - _t_note_on - _attack;
            _adsr_output = map(_decay_release_table[(int)floor(ARRAY_SIZE * (float) delta / (float) _decay)], 0, _vertical_resolution - 1, _sustain, _vertical_resolution - 1);
        }
        else
            _adsr_output = _sustain;
    }
    if(_t_note_off > _t_note_on)        {           // if note not pressed
        delta = l_micros - _t_note_off;
        if(delta < _release)                        // release
            _adsr_output = map(_decay_release_table[(int)floor(ARRAY_SIZE * (float) delta / (float) _release)], 0, _vertical_resolution - 1, 0, _release_start);
        else
            _adsr_output = 0;                       // note off
    }
    return _adsr_output;
}
