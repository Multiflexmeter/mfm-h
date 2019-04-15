# Multiflexmeter-H Sketch V3.3.0

This project contains the source code for the latest Multiflexmeter.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

The project is developed using the PlatformIO (PIO) environment. It is highly recommended to develop in VSCode, which has great PIO intergration.

 * [Visual Studio Code](https://code.visualstudio.com/)

Install PIO through the VSCode extensions, or follow a guide at the [PlatformIO Website](https://docs.platformio.org/en/latest/ide.html#).


### Installing

Clone this repository through git

```
git clone git@github.com:Multiflexmeter/mfm-h.git
```

Install PIO Libraries

```
pio lib install
```

Either program using the PIO controls in VSCode, or through a shell

```
pio run -t program -e m328p8m
```

## Deployment

Add additional notes about how to deploy this on a live system

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