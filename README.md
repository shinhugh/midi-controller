# MIDI Controller 10x6

Currently in the process of building a MIDI controller. It will mimic the
[Launchpad](https://novationmusic.com/en/launch) or
[MIDI Fighter](https://www.midifighter.com) while catering more to my specific
needs as a producer. Naturally, this means that the MIDI mapping will fit that
of FL Studio's performance mode.

The number of rows being greater than the number of columns is not arbitrary.
FL Studio allows only a single pattern to be triggered per row in its
performance mode and there can be at most 10 octaves in MIDI (at least as far as
FL Studio is concerned). Thus, 10 rows are used (instead of the standard 8) to
maximize simultaneous pattern triggering. As for the number of columns, 6 was
rather arbitrary; I just did not want the controller to be too large (and
buttons are pretty expensive).

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
which will not be included in the final model. Regardless, it serves as a useful
debugging tool.

The Arduino language is not used; everything is coded in "pure" C, using AVR's
libc.

[8/30/2020]
![Update photo](/photos/photo_20200830_0.jpg)

[8/28/2020]
![Update photo](/photos/photo_20200828_0.jpg)
![Update photo](/photos/photo_20200828_1.jpg)

[8/10/2020]
![Update photo](/photos/photo_20200810_0.jpg)
![Update photo](/photos/photo_20200810_1.jpg)
