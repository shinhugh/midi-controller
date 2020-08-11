# Arduino Experiments

Experimenting with the ATmega328P chip on the Arduino Uno in "pure" C.

Currently in the process of building a MIDI controller. It will mimic the
[Launchpad](https://novationmusic.com/en/launch) or
[MIDI Fighter](https://www.midifighter.com) while catering more to my specific
needs as a producer. Naturally, this means that the MIDI mapping will fit that
of FL Studio's performance mode.

As of 8/10/2020, the prototype is complete!

**Basic diagram**: Buttons ->
[MCP23017](http://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf) ->
[328P](http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf) ->
[16U2](http://ww1.microchip.com/downloads/en/DeviceDoc/doc7799.pdf) ->
Host PC

**328P**: Logic to handle hardware interaction and output the corresponding MIDI
events via serial (TTL)

**16U2**: [Hiduino](https://github.com/ddiakopoulos/hiduino) as a TTL to USB
converter (USB MIDI class-compliant device)

Also includes a basic text LCD with the
[HD44780 controller](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf),
which may or may not be included in the final model. Regardless, it serves as a
useful debugging tool.

[8/10/2020]
![Update photo](/photo_08102020_0.jpg)
![Update photo](/photo_08102020_1.jpg)
