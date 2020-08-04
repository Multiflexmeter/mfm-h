# Setting up the development environment

These instructions will get you a full development environment running. At the end of this guide you will be able to edit and build the source-code.

## Requirements

This is a list of required software. This guide includes their setup, but not their installation. See the software its website for the installation guide.

- **Git** - [Download](https://git-scm.com/downloads) - *Version control*
- **Visual Studio Code (VSCode)** - [Download](https://code.visualstudio.com/download) - *The code editor.*
- **PlatformIO (PIO)** - [Download](platformio.md) - *A development framework for embedded chips.*

## Clone

Now that all the tools are installed, let's get a copy of the project on your computer.

Open a shell on Linux, or open Git Bash on Windows and enter the following command:

```
git clone https://github.com/Multiflexmeter/mfm-h.git
```

This will clone (download) the project to your computer in a folder named "mfm-h".

You have succesfully cloned the project to your computer!

## Opening the project

This step will open the project in Visual Studio Code. There are two ways to achieve this. Either through a shell (like Git Bash) or the desktop. We will keep referring to this as Shell and GUI, respectively.

### Shell

Open a shell and navigate to the project folder on your computer. Once inside the folder open it in Visual Studio Code using the `code` command and supply a directory. (*Note that a dot means the current directory, and you just navigated to the project folder.*)

```
code .
```

### GUI

Open Visual Studio code through the desktop (e.g. the start menu or a desktop icon). Click *file -> Open Folder*, then navigate to and select the project folder.

## Building / Compiling

When you write code you need to compile it into something the chip can understand. The process of compiling is also called building the project.

There are again two ways to build the project.

### GUI

Click on the PlatformIO icon (looks like an ant) in the sidebar. Then in the new screen, under "Project Tasks" click build. This will build all environments.

To build a specific environment, under "Project Tasks" unfold the desired environment to build (indicated by "env:") and click build.

### Shell

PlatformIO has an extensive Command Line Interface (CLI). Use the following command to build the project. Leave out `-e environent_name` for all environments.

```
pio run -e 328p8m
```

# Next steps
- [Setting up Hardware](hardware.md)
- [Running tests](testing.md)
