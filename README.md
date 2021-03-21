### Valden: Heat Pump Controller v1.x
<b>  The Valden Heat Pump controller is a platform to precisely control heat 
pumps. Controller can be easily used for newly designed heat pumps (HPs), including series, as repair controller or as control system for experiments with refrigeration equipment.</b>
<br><br>

!!!GENERATE TOC HERE!!!

## Specs
- 12V 0.5A DC power supply, 
- 230V output
- up to 12 T sensors, -55..+125 °C range,
- 4 relays (<sup>Compressor, Hot CP or Air Fan, Cold CP or Air Fan, Crankcase Heater</sup>)
- 2 inputs (<sub>Hot and cold side refrigerant over/under pressure NC sensors</sub>)
- 6 pin EEV connection <sub>(4 * coils + 2 * 12V)</sub>
- control via remote display or local Serial(UART 5V)
- automatically turns on/of system when heating required
- takes care of system components, built-in protections: cold start, overheat, short-term power loss, power overload, ground loop freeze and compressor protection against liquid.
<br><br>

## Refrigeration schemes supported
- Heat Pump (HP) with Electronic Expansion Valve (EEV)
- HP with capillary tube,
- EEV only controller
<br><br>

## Installations supported
- Indoor (a house or technical building with a more or less stable temperature)
- Outdoor (harsh climatic conditions taken into account and tested down to minus 32°C)
<br><br>

## Changelog and history
- 2018: PCB prototype, first real installation, 
- 2019: 2-layer PCB, through-hole components, integrated buttons and display,
- 2019: controller redesigned taking into account development and operating experience,2-layer PCB, SMD
- 2019-2021: installations, development, tests, revisions, redisigns
- 06 Feb 2021: product is technically complete and ready for customers to use. Documentation and release stage.

## Get your own copy and PCB assembly
- download the PCB gerber file, {-_Gerber.zip-}
- find in Google, where to order a printed circuit board (keywords: order pcb gerber), place an order,
- order electronic components, see BOM (Bill Of Materials) list, {- _PCB_BOM.html-}
- solder electronic components {- assembly instructions here-}

## Firmware upload
The process is the same as for others Arduinos 
- connect USB-> UART converter 
- start Arduino IDE
- open the firmware file
- select board and MCU in the Tools menu (hint: we are using 328p, 16Mhz, 5V)
- press the "Upload" button in the interface and "Reset" on the Arduino

For arduinos with old bootloader you need to update it. (Tools-> Burn Bootloader).<br>
For successful compilation, you must have "SoftwareSerial", "OneWire" and "DallasTemperature" installed (see Tools -> Manage Libraries).<br>
As a first try it's enough to simply upload firmware without any tunings. Think of it as of commercial closed-source controller, where you cannot change internal options.

## Self-tests
QA tests are available to test the assembled board.<br>
Self-test helps you check relays, indicators, speaker and temperature sensors.<br>
To run a self-tests
- uncomment this 3 defines in source code header
```c
//-----------------------USER OPTIONS-----------------------
#define SELFTEST_RELAYS_LEDS_SPEAKER
#define SELFTEST_EEV
#define SELFTEST_T_SENSORS
```
- upload firmware
- connect 12V power supply<br>

Video {- demostration "How self-tests works":-}
To check EEV, you can use a stepper motor as shown in the video. If you are testing a real EEV, it should be closed after the first "beep" and partially opened after the second "beep".<br>
To check temperature sensors connectors prepare one array of sensors. Connect it to all sensors connectors one-by-one and check results in a serial console.<br>
{-Photo: PCB with connected arrays of sensors (I have enough sensors, so all 3 arrays are connected except Ts2 and Treg) -}<br>
{-Screenshot: serial console with temperature readings: -}<br>

## Wiring and installation
Wiring is very simple despite the many connections: <br>
Phases (1st wire in electrical cables):
- connect the "power inlet" wire to one of the "phase" connectors
- and connect the second "phase" connector to the AC input of the 12V power supply
- connect the "Compressor" relay output to the Compressor input
- connect the relay output "Hot CP" to the input of the Hot Circulation Pump (pump of the water floor heating system or the fan input of the indoor unit if you are using an air system)
- connect the relay output "Cold CP" to the input of the Cold Circulation Pump (ground loop pump for geothermal systems or the inlet of the outdoor unit air fan if you are using an air system)
- when using a compressor heater: connect the "Crankcase heater" output to the heater cable input (highly recommended for outdoor installation and year-round use)
Neutral (2nd cable in electrical cables):
- connect all the second wires to the "neutral" connectors on the board

{-Photo of HV connections-}<br>

Crimp and connect low-voltage connectors:
- SCT013 sensor (the only low-voltage device in the circuit with interchangeable contacts), connect and install on the phase inlet wire
- RS485 through a wire of the desired length to the remote control display (if used, another control method is a local computer with a USB-UART converter, you may like it for the first time). Note that A is connected to A in the display, B to B and GND to GND
- you can power the display from a 12V controller, the board has 12V and GND secondary pins
- EEV to EEV connector
- T sensors to T sensors connectors
- connect the outputs of the pressure sensors: 1st wires together to the right pin, 2nd cold side wire to the left terminal, 2nd hot side wire to the middle terminal; use the dummy when pressure sensors are not in use.

You may prefer to solder the wires over using connectors. But in this case, it will be more difficult to disassemble the system if you want to change something. The choice is yours.<br>
{-Photo: power wire + SCT013-}<br>
{-Photo: T sensors with abbreviations and full names-}<br>
{-Photo: RS485 and 12V power for display-}<br>
{-Photo: all together-}<br>
{-Photo: ? a crankcase heater looks like ? -}<br>

## Starting up the heat pump system for the first time and charging refrigerant
This is not a hard part, but if you don't have experience it will take time. <br>
You have performed a pressure test and vacuumated your system. It's time to charge.<br>
Let's say you don't know how to calculate the amount of refrigerant in a newly built system:
- charge a small amount (for example 300 g) of refrigerant
- start the heat pump
- at this moment the suction temperature (according to the suction pressure on the pressure gauge) will be about -20 ...- 40 ° C
- for single-component refrigerants: slightly open the valve of the HVAC gauge manifold to start adding refrigerant through the gas phase on the cold side,
- for multi-component refrigerants: turn over the refrigerant cylinder, VERY SLIGHTLY open the HVAC manifold valve to start adding a VERY LITTLE amount of refrigerant through the liquid phase
- carry out charging until the suction temperature (according to the suction pressure on the manometer) is ~ 10 ... 15 ° C lower than the temperature of the heat source (example: the temperature at the inlet of the mixture of water and antifreeze from the closed ground loop is + 8 ° C, so the suction temperature should be -2 ..- 7), then close the manifold valve,
- wait for the system to heat the target to a more or less stable temperature, add refrigerant while temperature increases (and suction decreases)
- stop if you are sure that the heating process is now proceeding very slowly
- check the discharge pressure: it should not be too high compared to the temperature from the discharge sensor (Tbc) of the hot side.
- Carry out the final charge when the system is stable and the heat pump stops when the set point is reached (normal stop), this may take 12 hours or more, and now the target difference between the suction pressure temperature and the temperature from the T sensor should be 3 ... 6 ° C,

This algo is good and safe both the first time and as a starting point. As you gain experience, you will get yours much easier and faster. {-Also try "manual EEV mode" during charging process.-}
{-?manifold photos with comments?-}

## Control and usage: remote control unit
{-There are 2 ways to control heat pump-}
qwer

## Control and usage: serial console
{-desc 2-}
qwer

## Fine tunings
{-link to page-}
qwer

## Diagnostic and troubleshooting
{-describe messages-}
qwer

## T sensors abbreviations
Abbreviations used in the documentation and firmware <br><br> 

Main sensors:
| Abbr. | Full name             |
| ----- | --------------------  |
| Tae   | after evaporator      | 
| Tbe   | before evaporator     |
| Tci   | cold side "input"     |
| Tco   | cold side "output"    |
| Tbc   | before condenser      |
| Tac   | after condenser       |
| Thi   | hot side "input"      |
| Tho   | hot  side "output"    |
| Tcrc  | crankcase (compressor itself)|

Additional sensor used in "swiming pool heater" or "water tank heater" schemes:
| Abbr. | Full name             |
| ----- | --------------------  |
| Ts1   | additional sensor1    |

Additional sensors, disabled and not used by default:
| Abbr. | Full name             |
| ----- | --------------------  |
| Treg  | regenerator temperature|
| Ts2   | additional sensor2    |

## Hints 


For more information about Heat Pumps look at [Wikipedia about HP](https://en.wikipedia.org/wiki/Heat_pump).<br>
If you are interested in questions like "how refrigeration systems works" read Patrick Kotzaoglanian books.<br>
If you want more technical details, sophisticated scmemes, "how EEV can be driven by temperature" diagrams, etc. refer to vendors manuals (you can find a lot in Alfa Laval brochures, Danfoss guides, and so on).<br>
For refrigerants and oils types comparison see wiki.<br>

## Personal experience
Note that the SCT013 sensor and the current monitoring scheme cannot be used for accurate measurements and accurate COP calculations. Use wattmeter for accurate power measurements.<br>
Measuring the temperature of a warm floor with sensor at one point is a bad idea - it's better to deal with temperature of the "hot in" water coming from all over the floor, as implemented in firmware.<br>
The weather-dependent  (both outdoor and indoor temperature dependent) system does not work fine for 30-150 m2 buildings. The system turns out to be difficult and works bad due to unpredictable ventilation. And also due to the unpredictability of heat emitted in the house by other sources.<br>
I tried the scheme with a flooded evaporator in 2019 and found it extremely problematic, then refused to use it.<br>
Deep regeneration schemes are useful only for some refrigerants and only in certain temperature ranges. I also tried deep regeneration, I was convinced that the theory coincides with the practice and then also refused this idea.<br>
In general, it is possible by complicating the refrigeration scheme to win somewhere 1%, somewhere 3%, but all this leads to significant time and money costs, compared to not very much profit.<br>
Summary: If you want experiments - Experiment. Want reliably and quickly - make system simple.<br>

## License

GPLv3. <br><br>

This product is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

## Author
gonzho АТ web.de (c) 2018-2021<br><br>
