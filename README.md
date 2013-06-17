j1850emitter
============

Arduino that emits J1850 messages used for testing of j1850decoder.
I used a 16MHz Arduino Uno, but any Arduino should do if the ports are checked..

By pulling input 7/6/5 to gound you can choose between message patterns, sent each 800mSec.
Harley Davidson Sportster OBD2 data within parenthesis:

p# 7 6 5
--------
   - - -  0000 (zeros)
   - - X  ffff (ones)
   - X -  281b10021000 (rpm)
   - X X  482910020700 (speed)
   X - -  a83b10030f (gear)
   X - X  a8836112d8 (fuel gage)
   X X -  a849101085 (engine temp)
   X X X  The bytes below with 50mSec delay between
          2B1B10021000
          482910020700
          A83B10030F
          A849101085
          48DA403901 (turn left)
          48DA403902 (turn right)
