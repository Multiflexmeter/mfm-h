# Using the serial monitor

The serial monitor is a software developer its best friend. It can be used to provide state information or verification that a certain code-block has been reached.

## Setup

Before one can use the serial monitor, there must be a UART conversion to your computer. We recommend using something like the FT232RL.

See the [hardware setup guide](setup/hardware.md) on how to connect the FT232RL to the Multiflexmeter board.

## Usage

The serial monitor can be opened in two ways, either through the GUI or the CLI.

### GUI

In VSCode click the PlatformIO icon (looks like an ant) and under "Project Tasks" click "Monitor".

### CLI

Enter the following command.

```
pio device monitor
```

### Settings

The serial monitor settings can be changed using the appropriate keys. Press `CTRL+T , CTRL+H` for help.

When using the CLI you can append settings. For example, the following will change the baud rate to 115200.

```
pio device monitor -b 115200
```