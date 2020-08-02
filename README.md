# Arduino Experiments

Experimenting with the ATmega328p and ATmega16U2 chips on the Arduino Uno in
"pure" C.

Currently in the process of building a MIDI controller. It will mimic the
[Launchpad](https://novationmusic.com/en/launch) or
[MIDI Fighter](https://www.midifighter.com) while catering more to my specific
needs as a producer. Naturally, this means that the MIDI mapping will fit that
of FL Studio's performance mode.

**Basic diagram**: Buttons -> 328p -> 16U2 -> Host PC

**328p**: Logic to handle hardware interaction and output the corresponding MIDI
events via serial (TTL).

**16U2**: [Hiduino](https://github.com/ddiakopoulos/hiduino) as a TTL to USB
converter (USB MIDI class-compliant device).

[8/02/2020]
![Update photo](/update_photo.jpg)
