# nsnds

Software to use your Nintendo DSi and Raspberry Pi Pico W as a controller for a Nintendo Switch or Steam.

## Requirements

- A Nintendo DSi with a way to boot a ROM
- A Raspberry Pi Pico W

## Setup

- Install [devkitPro](https://devkitpro.org/wiki/Getting_Started)
- Install [the Arduino IDE](https://www.arduino.cc/en/software) and [the rp2040 core (version 2.6.5)](https://github.com/earlephilhower/arduino-pico)
- Copy `boards.local.txt` to your RP2040 folder (`%LOCALAPPDATA%\Arduino15\packages\rp2040\hardware\rp2040\2.6.5\` for Windows)
- Set `Tools->USB Stack` to `Adafruit TinyUSB` in the Arduino IDE
- MANUALLY configure the ONLY available network on the Nintendo DS to have the following options:
	- SSID: value of `NSNDS_SSID` in `nsnds.h` (default: `nsnds`)
	- Security: None
	- IP Address: 192.168.42.16
	- Subnet Mask: 255.255.255.0
	- Gateway: 192.168.42.1
	- Primary DNS: 8.8.8.8
	- Secondary DNS: 8.8.4.4
	- MTU Value: 1400
	- The connection test is supposed to fail
	- I'm not sure which of these values are necessary, but they work so I'm not touching them
- Run `make` in the terminal (Unix) or in MSys2 (Windows) to build the DS ROM (`nsnds.nds`), then move it to your SD card.
- Open `pico` as a sketch in the Arduino IDE and upload it to your Pico W.

## Usage

1. Plug in the Pico W to a Nintendo Switch or PC
2. Launch the ROM on the Nintendo DS WHILE the Pico W is plugged in and the Nintendo Switch or PC is on

The connection will break when the Nintendo Switch goes into sleep mode or after a while if too many Bluetooth controllers are connected.
In such a case, just restart the ROM on the DS.
