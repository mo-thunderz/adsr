# ADSR
ADSR based on lookup table for Arduino Due (and other Arduinos). Advantages of this implementation are that it is extremely light-weight, has a differential profile and that the attack, decay and release times can be precisely controlled.

See as well my youtube video on how to use this class:
https://youtu.be/oMxui9rar9M

# Installation
1) Make a new folder called "adsr" in your arduino libraries folder
2) Copy adsr.cpp and adsr.h into the "adsr" folder

# How to use
Please refer to the ADSR_example.ino file in the examples folder. This file is written for the Arduino Due and can be run on other Arduino boards with some small modifications. I used the Due as it has a built-in DAC.
NOTE: you can change the shape of the ADSR by changing the ATTACK_ALPHA and ATTACK_DECAY_RELEASE parameters in adsr.h

# Updates
* 29.12.2020: fixed issue for very long delay times (conversion to float was needed). Delays up to 20s verified.
* 14.08.2022: added a function setResetAttack(bool l_reset_attack); -> By setting the reset attack to true (default is false) the ADSR will always start at 0 when a new trigger is received. In the default setting a new trigger starts at the current output value.


Have fun ;-)

mo thunderz
