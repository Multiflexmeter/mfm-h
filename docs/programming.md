# Programming the Multiflexmeter

Whether you modify the source-code or the source-code settings, you will need to recompile and reprogram the chip on the Multiflexmeter. 

PlatformIO provides excellent support for programming microchips in various ways. It seperates different programming settings through "environments" that you can set up.

We have provided a PlatformIO environment that programs through an ISP. It was set up for the Diamex ISP, but should work with an "Arduino as ISP" as well.

# Setup

Before being able to program the chip, you will need a connection to the chip. The chip is programmed through the SPI headers provided on the board.

See the [hardware guide](setup/hardware.md) on how to connect a programmer to the SPI header.

# Programming