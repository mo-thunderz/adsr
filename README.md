# ADSR
ADSR based on lookup table for Arduino Due (and other Arduinos). Advantages of this implementation are that it is extremely light-weight, has a differential profile and that the attack, decay and release times can be precisely controlled.

# Installation
1) Make a new folder called "adsr" in your arduino libraries folder
2) Copy adsr.cpp and adsr.h into the "adsr" folder

# How to use
Please refer to the ADSR_example.ino file in the examples folder. This file is written for the Arduino Due and can be run on other Arduino boards with some small modifications. I used the Due as it has a built-in DAC.
NOTE: you can change the shape of the ADSR by changing the ATTACK_ALPHA and ATTACK_DECAY_RELEASE parameters in adsr.h

Have fun ;-)

mo thunderz
