# MEGAPRO
An open-source high-performance  EEPROM programmer for 28CXX series chips.

*Tested on AT28C256, should also work with other 28CXX EEPROM in theory but compatibility is not guaranteed.*


## Usage
1. Upload `megapro-arduino/megapro-arduino.ino` to your Arduino Mega 2560 using Arduino IDE.
2. Connect your Arduino with EEPROM based on schematics or print a PCB shield.
3. Use `python3 com.py [-h] [--port PORT] [--rom ROM]` to program your EEPROM with specified firmware.

```
usage: com.py [-h] [--port PORT] [--rom ROM]

Programming script for sending pages data into Arduino Mega 2560.

optional arguments:
  -h, --help            show this help message and exit
  --port PORT, -p PORT  Serial port of Arduino Mega 2560
  --rom ROM, -r ROM     Path to rom file.
```

## TODO
+ Confirm compatibility with other 28CXX chips.
+ Make the script an executable file.
+ New hardware design for supporting other EEPROM series.
