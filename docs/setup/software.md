# Setting up the software

These instructions will help you configure the software for your device. At the end of this guide you should be able to configure the Multiflexmeter to send to your LoRaWAN network.

## Device-keys

To have your Multiflexmeter send information to your network, you require a set of keys. These are obtainable through the network you chose.

The Multiflexmeter uses the Over-The-Air-Activation (OTAA). This means the encryption keys are refreshed everytime your device resets.

These keys are compiled into the sketch through compiler-definitions. This means they can only be changed by reprogramming the device.

The keys are defined in the `lib/MFMLora/MFMLoraConfig.h` file. However they can also be defined compile time by providing the compiler with a `-D` parameter, to redefine the variable.

```cpp
// Define your LoRaWAN keys here
#define _DEV_EUI { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define _APP_EUI { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define _APP_KEY { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
```

**Note: that both DEV_EUI and APP_EUI are LSB, while APP_KEY is provided as is. (TTN = MSB)** 

## Send interval / Sleep duration

The multiflexmeter does not send every X minutes. Instead it sends, waits for completion and sleeps X minutes.

*This will change in future versions, however the current version is missing an RTC module.*

The duration is defined as sleep iterations. One sleep iteration is 8 seconds. This setting can be configured in the `lib/MFMLora/MFMLoraConfig.h` file. 

# Next steps

- [Programming the device](programming.md)