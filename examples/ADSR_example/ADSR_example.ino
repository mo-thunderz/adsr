// --------------------------------------------------
//
// ADSR - based on lookup table - Arduino Due
// by mo_thunderz (mo-thunderz @github)
//
// this example periodically triggers the adsr and has been written
// for the Arduino Due. For use on other boards just reduce the
// DACSIZE, remove the analogWriteResolution(12) statement and write
// to a different port in this statement:
// analogWrite(DAC0, adsr_class.getWave(t));
//
// Parameters:
// *) trigger_duration = duration of the adsr trigger (in µs)
// *) space_between_trigger = duration between triggers (in µs)
// *) adsr_attack = attack time (in µs)
// *) adsr_decay = decay time (in µs)
// *) adsr_sustain = level for sustain (0 to DACSIZE - 1)
// *) adsr_release = release time (in µs)
//
// --------------------------------------------------

#include <adsr.h>                                   // import class

#define DACSIZE 4096                                // vertical resolution of the DACs

// variables
unsigned long   adsr_attack = 100000;               // time in µs
unsigned long   adsr_decay = 100000;                // time in µs
int             adsr_sustain = 2500;                // sustain level -> from 0 to DACSIZE-1
unsigned long   adsr_release = 100000;              // time in µs
unsigned long   trigger_duration = 300000;          // time in µs
unsigned long   space_between_triggers = 500000;    // time in µs

// internal variables
bool trigger_on = false;                            // simple bool to switch trigger on and off
unsigned long   t = 0;                              // timestamp: current time
unsigned long   t_0 = 0;                            // timestamp: last trigger on/off event

// internal classes
adsr            adsr_class(DACSIZE);                // ADSR class initialization

void setup() {
  delay(100);
  analogWriteResolution(12);                        // set the analog output resolution to 12 bit (4096 levels) -> ARDUINO DUE ONLY

  pinMode(LED_BUILTIN, OUTPUT);                     // initialize LED

  adsr_class.setAttack(adsr_attack);                // initialize attack
  adsr_class.setDecay(adsr_decay);                  // initialize decay
  adsr_class.setSustain(adsr_sustain);              // initialize sustain
  adsr_class.setRelease(adsr_release);              // initialize release
}

void loop() {
  t = micros();                                     // take timestamp

  analogWrite(DAC0, adsr_class.getWave(t));         // update ADSR and write to DAC

  if (trigger_on) {                                 
    if (t > t_0 + trigger_duration) {               // check if trigger can be switched off
      adsr_class.noteOff(t);                        // inform ADSR class that note/trigger is switched off
      digitalWrite(LED_BUILTIN, LOW);               // switch off LED
      t_0 = t;                                      // reset timestamp
      trigger_on = false;                           // set trigger_on to false
    }
  }
  else {
    if (t > t_0 + space_between_triggers) {            // check if trigger can be switched on
      adsr_class.noteOn(t);                         // inform ADSR class that note/trigger is switched on
      digitalWrite(LED_BUILTIN, HIGH);              // switch on LED
      t_0 = t;                                      // reset timestamp
      trigger_on = true;                            // set trigger_on to false
    }
  }
}
