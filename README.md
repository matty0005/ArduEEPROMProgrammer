# ArduEEPROMProgrammer
An Arduino Uno based programmer for the 28C256.

![alt text](https://raw.githubusercontent.com/matty0005/ArduEEPROMProgrammer/master/ArduEEPROMProgrammer.jpg)

# Usage:
To read EEPROM, enter `r` into the serial monitor.
To erase, enter `e`, or enter `w` to enter write mode

Once in write mode, first enter the value to be stored, followed by a space, eg `28 `
It will then ask for the address to store the value. Enter it in the same format

### LEDs
There are three LEDs on this sheild - all are optional. The far right one (red) is simply a power indicator, while the other two are connected to analog pin 2 (Green) and analog pin 3. Currently they are programmed to indicate if it is writing (green) or writing (blue) to the EEPROM.


# Hardware
I designed the PCBs and had them made by JLCPCB. Although, any PCB manufacturer will do.
Here is a link to the [EasyEDA project](https://easyeda.com/matty0005/ardueepromprogrammer). Alternatively, you can download the gerber files yourself too.
## Schematic and Board layout

![alt text](https://raw.githubusercontent.com/matty0005/ArduEEPROMProgrammer/master/SchematicAndBoard.png)

## Build Of Materials
This board uses a 40Pin ZIF socket, two 74HC595 8bit shift registers (RS part no. 773-7796), three 5mm LEDs (optional) and accompanying 470ohm resistors.
