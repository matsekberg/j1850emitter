j1850emitter
============

Arduino that emits J1850 messages used for testing of j1850decoder.
I used a 16MHz Arduino Uno, but any Arduino should do if the ports are checked..

By pulling input 7, 8 or 9 you can choose between three message types.
- 7: only ones,
- 8: only zeros,
- 9: a mixed message.
