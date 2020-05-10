# NMEA 2000 KiCAD PCB and sketch for analog inputs (e.g. water level, fuel gauge, switch, etc ...)

Based on https://github.com/ttlappalainen/NMEA2000

Can be used to create a NMEA 2000 device like a sensor for fuel gauges. The board is easy to work with since it contains ISP and serial ports. 

*Use at your own risk. Don't forget the right external fuses, etc.* 

![RENDERED](/sensor.jpg)

![PHOTO](/sensor2.jpg)

## How to

### PCB

Equip SMD parts for power (LDO, etc) first and measure input to output voltage carefully. Adjust R4 (and R3) resitors, if necessary. If voltage is around 4.8V - 5V, place other parts. 

### Burn bootloader

0. Disconnect PCB from other power sources or adapters.
1. Connect an AVR ISP programmer (usbasp) to ISP port
2. Change into arduino-blink directory and type ```pio run --target fuses```. If the chip doesn't answer run avrdude manually with ```-B``` flag to set the bit clock rate, see  https://electronics.stackexchange.com/questions/388324/avrdude-error-program-enable-target-doesnt-answer-1
3. Change into arduino-blink directory and type ```pio run --target bootloader```
4. Optional: The sketch can be uploaded via ISP directly from arduino-sensor directory (```pio run --target program```)

### Upload sketch

0. Disconnect PCB from other power sources or adapters.
1. Connect an USB serial adapter to serial port
2. Change into arduino-sensor directory
3. Run ```pio run --target upload```

If errors appear during upload (e.g. ```programmer is out of sync``` or ```protocol error``` use another USB cable, USB hub or USB adapter. Check USB voltage (>4.89V) and ripple noise.

## BOM

- Case: RND 455-00182 
  https://www.reichelt.de/kunststoffgehaeuse-115-x-65-x-40-mm-ip65-rnd-455-00182-p193396.html
  https://www.distrelec.de/Web/Downloads/_t/ds/RND_455-00182_30064403_mcd_eng_tds.pdf

- AVR: AT90CAN128
  https://www.reichelt.de/atmel-ic-at-90can128-p55544.html

- Transceiver: MCP 2562-E/SN
  https://www.reichelt.de/bus-controller-1-treiber-1-empfaenger-so-8-mcp-2562-e-sn-p143745.html

- Connector: M12 Plug
  https://www.reichelt.de/sal-m12x1-5-pol-einbaustecker-sal12-fs50-p136981.html?&trstct=pol_38&nbc=1

- 12V to 5V: LDO 
 https://cdn-reichelt.de/documents/datenblatt/A200/LT3010EMS8E5.pdf 
