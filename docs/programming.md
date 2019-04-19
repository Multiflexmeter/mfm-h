# Programming the Multiflexmeter

Whether you modify the source-code or the source-code settings, you will need to recompile and reprogram the chip on the Multiflexmeter. 

PlatformIO provides excellent support for programming microchips in various ways. It seperates different programming settings through "environments" that you can set up.

We have provided a PlatformIO environment that programs through an ISP. It was set up for the Diamex ISP, but should work with an "Arduino as ISP" as well.

# Setup

Before being able to program the chip, you will need a connection to the chip. The chip is programmed through the SPI headers provided on the board.

See the [hardware guide](setup/hardware.md) on how to connect a programmer to the SPI header.

# Programming

Programming through PlatformIO is very easy. The project contains an environment for ISP programming called "328p8m". This environment programs the AtMega328p at 8MHz.

You can program the device through the GUI and CLI.

## GUI

In VSCode click on the PlatformIO icon (looks like an ant), then under "Project Tasks" select "env:328p8m" and click on "Program". A window should appear that logs the programming progress.

## CLI

Using a shell, change directory to the project folder and execute the following command:

```
pio run -t program -e 328p8m
```

The shell window should now output the programming progress.