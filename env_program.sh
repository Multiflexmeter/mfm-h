#!/bin/bash

source .env
export PLATFORMIO_BUILD_FLAGS="-DDEV_EUI=$DEV_EUI -DAPP_EUI=$APP_EUI -DAPP_KEY=$APP_KEY"

pio run -t program -e 328p8m -v