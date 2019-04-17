# Multiflexmeter-H Sketch

This project contains the source code for the latest Multiflexmeter.

## Introduction

The word *Multiflexmeter* is a Portmanteau of *Multifunctional, Flexible, Meter*.

- **Multi;** Capable of measuring different properties
- **Flex;** Modifiable to your needs
- **Meter;** *Dutch noun*: Measuring Device

The Multiflexmeter is a fully Open-Source project. Each necesarry component to build your own is available online.

- [The Github organisation](https://github.com/Multiflexmeter)
- [The wiki](https://wiki.multiflexmeter.net)
- [The website](https://multiflexmeter.nl)

The Multiflexmeter is developed with an AtMega328p. This is the same chip used on an Arduino Uno. This is too keep the entry level low for new developers.

## Getting Started

These instructions will get you a full development environment up and running.

### Prerequisites

This project is developed using the [PlatformIO (PIO)](https://docs.platformio.org/en/latest/ide.html#) software. We highly recommend using it with [Visual Studio Code](https://code.visualstudio.com/).

**Software**

- [Visual Studio Code](https://code.visualstudio.com/)
  - *Tip: Use `` CTRL+` `` for a command shell.*
- [PlatformIO (PIO)](https://docs.platformio.org/en/latest/ide.html#)

**Hardware**

- A MFM-H V3 Board
- FTDI Programmer
- SPI Programmer

**Other**

- Nearby LoRaWAN gateway
- [LoRaWAN OTAA Device keys](https://www.thethingsnetwork.org/docs/devices/registration.html)


### Installing

Installation is very straightforward.

Clone this repository through git

```
$ git clone git@github.com:Multiflexmeter/mfm-h.git
```

Open Visual Studio Code on the project

```
$ cd mfm-h
$ code .
```

If there are native tests available, you could execute them through  
*A native test, tests code that is not dependant on a microchip or the Arduino library.*

```
$ pio test -e native 
```

## Programming the board

The PIO platform takes care of most business. The `platformio.ini` file defines the possible environments for programming and testing. The only available programming environment currently is called `328p8m`. 

Connect the SPI Programmer to the board and run the program command with the 328p8m environment through PIO

```
$ pio run -t program -e 328p8m
```

### Monitoring the board (Serial communication)

To establish serial communication you need to connect the TX and RX pins on the board to your computer. An easy way to accomplish this is using a FTDI programmer.

Connect GND, TX and RX from the FTDI programmer to the board and plug the FTDI's USB in your computer. Run the following command to open a serial communication window.

```
$ pio device monitor
```

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/multiflexmeter/mfm-h/tags). 

## Authors

* **Thian Kaag** - *Initial work* - [Thian99](https://github.com/Thian99)
* **Jos Goossen** - *Initial work* - [Flexjos](https://github.com/Flexjos)
* **Tim van Osch** - *Initial work* - [Timvosch](https://github.com/timvosch) | [Pollex\'](https://github.com/Pollex)

See also the list of [contributors](https://github.com/multiflexmeter/mfm-h/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Acknowledgments

* Thanks to [PurpleBooth](https://gist.github.com/PurpleBooth) for the [ReadMe template](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2).