## UML

## API documentation

## System architecture

## Hardware architecture
Below you will find a simplified overview of the hardware in this project.
In the center there is a ESP866 that is powered by the HW-775.
The HW775 will charge the 1S Lipo. This lipo make is possible to use the devices a as standalone devices.
Charging the Lipo is done with a 5V input source.

The EPS has several devices connected to it. 
There is the TFT LCD, this is connected by the SPI bus.
The us of a SPI bus reduces the refresh rate.
Further there is WS2812 LED to indicate the rise or fall of the coin.
This LED Is controlled by a special library.
The reset button gives the user to clear the EEPROM on boot. Making the devices setup like fresh out of the box.

![My Diagram](hardware_architecture_overview.drawio.svg)
### Wiring diagram

### BOM