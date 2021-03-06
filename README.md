
## Valden Heat Pump Controller v1.x
<b>The Valden Heat Pump controller is an open source platform to precisely control heat pumps. This controller can be used for the automation of newly built Heat Pumps (HPs), as a repair controller for old systems or as control system for performing experiments on refrigeration equipment.</b>
<br><br>

## Specs
- 12V 0.5A DC power supply,
- 230V output,
- 4 16A relays: Compressor, Hot Circulating Pump (CP) or Air Fan, Cold CP or Air Fan, Crankcase Heater,
- 2 inputs: Hot and cold side refrigerant over/under pressure NC sensors,
- up to 12 temperature (T) sensors, -55..+125 °C range,
- Electronic Expansion Valve (EEV) supported, 6 pin EEV connection: 4 * coils + 2 * 12V,
- automatically turns on/of system when heating required,
- automatic power saving mode,
- built-in protections: cold start, overheat, short-term power loss, power overload, ground loop freeze, compressor protection against liquid and other,
- LED indication,
- control via [remote display](https://github.com/openhp/Display/) or local Serial (UART 5V).
<br><br>
<img src="./m_controller_and_display.jpg" width="800"><br><br>

## Refrigeration schemes supported
- Heat Pump (HP) with Electronic Expansion Valve (EEV),
- HP with capillary tube or TXV,
- EEV-only controller.<br><br>

## Installations supported
- Indoor: a house or technical building with an almost stable temperature,
- Outdoor: harsh climatic conditions taken into account. Outdoor HP installations tested down to a minus 32 °C.<br><br>

## Changelog and history
- 2018: PCB prototype, first real installation,
- 2019: 2-layer PCB, through-hole components, integrated buttons and display (public access),
- 2019: controller redesigned taking into account development and operating experience, 2-layer PCB, SMD,
- 2019-2021: installations, development, tests, revisions, redesigns (limited access), 
- 06 Feb 2021: product is technically completed and ready for public access. Documentation and release stage,
- 31 Aug 2021: public access granted.<br><br>

## Get your own PCB copy. Assembly.
- download PCB Gerber file [here](./Valden_HeatPumpController_Gerber.zip) or get your own copy [there](https://www.pcbway.com/project/shareproject/Valden_Heat_Pump_Controller_v1.html),
- order electronic components, see BOM (Bill Of Materials) appendix,
- solder electronic components, [assembly instructions here](https://github.com/openhp/HeatPumpController/wiki/Assembly)<br><br>.
<img src="./m_c_assembly_completed.jpg" width="500"><br><br>

## Firmware upload
This process is the same as for other Arduinos:
- connect USB-> UART converter,
- start Arduino IDE,
- download and open the [firmware file](./Valden_HeatPumpController.ino),
- select board and MCU in the Tools menu (hint: we are using "mini" board with 328p MCU),
- press the "Upload" button in the interface and "Reset" on the Arduino.

For arduinos with an old bootloader you need to update it. (Tools-> Burn Bootloader).<br>
For successful compilation, you must have "SoftwareSerial", "OneWire" and "DallasTemperature" libraries installed (see Tools -> Manage Libraries).<br>
For the first time it's enough to upload firmware without any tuning. Think of it as of a commercial closed-source controller, where you cannot fine-tune internal options. And any other manual configuration do not required too, just upload firmware. You will see an error LED indication and hear a beep, since no sensors connected to your controller. Follow the next steps.<br>
<img src="./m_add_IDE.png" height="300"><br><br>

## Self-tests
QA tests are available to test the assembled board.<br>
Self-test helps you check relays, indicators, speaker and temperature sensors.<br>
To run a self-tests:
- uncomment this 3 defines in source code header,
```c
//#define SELFTEST_RELAYS_LEDS_SPEAKER    //speaker and relays QA test, uncomment to enable
//#define SELFTEST_EEV                    //EEV QA test, uncomment to enable
//#define SELFTEST_T_SENSORS              //temperature sensors QA test, uncomment to enable
```
- upload firmware,
- connect 12V power supply,
- disconnect +5V wire from USB-UART converter.<br>

To check EEV connection, you can use a stepper motor.  If you are testing a real EEV, it will be closed after the first "beep" and partially opened after the second "beep". If it's not, check if stepper or EEV center pin(s) connected to +12V and try to swap coil-end pins (EEV1..EEV4).<br>
<img src="./m_c_selftest_EEV.jpg" width="500"><br>
To check temperature sensors connectors crimp one array of sensors. Plug it to all sensor connectors one-by-one and check results in a serial console.<br>
<img src="./m_c_selftest_t_sensors.jpg" width="500"><br>
<img src="./m_c_selftest_t_readings.png"><br>
After tests completed, comment 3 self-test defines.<br>
Choose your installation scheme and uncomment one of those options:
```c
#define SETPOINT_THI 	//"warm floor" scheme: "hot in" (Thi) temperature used as setpoint
//#define SETPOINT_TS1 	//"swimming pool" or "water tank heater" scheme: "sensor 1" (Ts1) is used as setpoint and located somewhere in a water tank
```
Re-upload firmware. Your controller is ready for the first start (after wiring). Probably you'll never need to change other options.<br><br>

## Wiring (permanent controller installation).
Here are no instructions for choosing the right placement for permanent installation of the controller. It depends. You're building your system, and you know much better "where" and "how".<br>
Assuming you have installed your controller to the permanent place, the next step is wiring.<br><br>
Wiring is very simple, despite a lot of terminals.<br>
Phases (1st wire in electrical cables):
- connect the "power inlet" wire to one of the "phase" terminals,
- connect the "Compressor" relay output to the Compressor input,
- connect the "Hot CP" relay output to the Hot Circulation Pump input (or to the fan power input of the indoor unit if you are using an air system),
- connect the "Cold CP" relay output to the Cold Circulation Pump input (or to the fan power input of the outdoor unit),
- when using a compressor heater: connect the "Crankcase heater" relay output to the heater cable (highly recommended for outdoor installation and year-round use),
- connect all the second wires of power cords to the "neutral" terminals on the board.<br>
<img src="./m_c_wiring_power.jpg" width="600"><br>

12V Power Supply:
- connect the second "phase" and one of "neutral" terminals to the AC input of the 12V power supply,
- connect 12V power supply output to GND and 12V<br>
<img src="./m_c_wiring_12v.jpg" width="600"><br>

Crimp and plug low-voltage connectors:
- crimp SCT013 sensor wires (the only one low-voltage device in this circuit with interchangeable wires), connect and install it on the inlet phase wire,<br>
<img src="./m_c_wiring_current_sensor.jpg" width="600"><br>
- crimp RS485 to the Remote Display, using a wire of desired length (note that A is connected to A, B to B and GND to GND),
- crimp  12V and GND secondary terminals to the remote display,<br>
<img src="./m_c_wiring_display.jpg" width="600"><br>
- connect EEV to EEV terminal,<br>
<img src="./m_c_wiring_EEV.jpg" width="600"><br>
- install all T sensors on pipes, insulate tubes,
- crimp T sensors arrays, you can crimp all four GND wires at every array to one GND connector pin or make 1-to-4 connection somewhere closer to sensors location (same for +5V wires),
- insert T sensors arrays to appropriate terminals (if you do not need to control over all temperatures, disable and do not install unnecessary sensors),<br>
<img src="./m_c_wiring_t_sensors.jpg" width="600"><br>
- crimp and plug pressure sensors outputs: crimp 1st wires together to **12V** (right output of the terminal), 2nd cold side wire to the **Pco** (left), 2nd hot side wire to the **Phi** (middle); use the dummy if no pressure sensors used in your system.
<img src="./m_c_wiring_pressure.jpg" width="600"><br>
<img src="./m_c_wiring_pressure_dummy.jpg" width="600"><br>

You may prefer to solder the wires over using terminals and crimping connectors. But in this case, it will be difficult to disassemble the system if you want to change something. The choice is yours.<br><br>
And one more: **remember! 230V inside!** Do not turn on the phase without need.<br>
Have you ever received 230V with your own hands? If yes - you know. If no - do not try.<br>
Also remember about animals and children during the installation at a permanent place.<br><br>

## Control and usage: serial console
This is a first interface to Heat Pump controller you'll see after uploading firmware (Tools->Serial Monitor).<br>
The console itself is simple to use, several commands are available. Type in command, press "Send". Help and hotkeys:<br>
![console help screenshot](./m_console_help.png)<br>
Every 30 sec. (**HUMAN_AUTOINFO** option) you'll see stats. For example, after a startup of your compressor, you'll see something like this:<br>
![console statistics screenshot](./m_console_stats.png)<br>
At this example, "hot in" ~30 °C, compressor ~80 °C and so on. Heat Pump (HP, compressor) ON, Hot water pump ON, Cold water pump ON. Power consumption 980 watts.
Abbreviations: refer to Appendix A below.<br>
Also, you'll see diagnostic messages in your serial console.<br>
Do not connect +5V wire from USB-UART converter, if you are using a serial console.<br><br>

## Control and usage: [Remote Control Display](https://github.com/openhp/Display/)
This is a way for the end user to control Heat Pump.<br>
<img src="./m_display_main.jpg" width="300"><br><br>
End user does not want to know much about refrigerants, evaporation, discharge temperatures and so on, so this display designed as simple as it was possible. See [Remote Display page](https://github.com/openhp/Display/) for details. And yes, this display is open product too, with available Gerber, PCB and source code.<br><br>

## Control and usage: [Service Display](https://github.com/openhp/ServiceDisplay/) 
One day I've realized that a netbook with a serial console is a good diagnostic tool, but I want a compact tool to get maximum available information from a Heat Pumps. So, this "Quickly Assembled Service Display" appeared. It fits everywhere and with a good power bank it can work 2-3 days long, without any additional power source. The diagnostic display is build from scratch, no PCB and housing here (and no plans to create it), because I do not see this service display as a permanently mounted device. <br>
<img src="./m_tft_mainscreen.jpg" width="300"><br><br>
If you want a compact and visual tool - this device is for you, so check the [Service Display Page](https://github.com/openhp/ServiceDisplay/) .<br><br>

## Starting up the heat pump system for the first time and charging refrigerant
This is an easy part, but if you don't have experience it will take time.<br>
You have performed a pressure test and vacuuming. It's time to charge your system.<br>
Let's say you don't know how to calculate the amount of refrigerant in a recently built system, so follow next steps:
- charge a small amount (for example 300 g) of refrigerant,
- get ready for a system protective stops by Tae or Tbe temperature, this is a normal system behavior while refilling refrigerant,
- power on your heat pump,
- after compressor startup suction temperature will be about -20 ...- 40 °C (according to the suction pressure on the pressure gauge),
- for single-component refrigerants: slightly open the valve of the HVAC gauge manifold and start adding refrigerant through the gas phase on the cold side,
- for multi-component refrigerants: turn over the refrigerant cylinder, VERY SLIGHTLY open the HVAC manifold valve and start adding VERY LITTLE amounts through the liquid phase,
- continue, until the suction temperature (according to the suction pressure on the manometer) is ~ 10 ... 12 °C lower than the temperature of the heat source (example: the temperature at the inlet of the mixture of water and antifreeze from the closed ground loop is + 8 °C, so the suction temperature should be -2 ..- 4),
- close the manifold valve,
- at every step check the discharge pressure: it should not be above the discharge sensor temperature (Tbc),
- wait for the system to heat the target to an almost stable temperature, add little amounts of refrigerant while temperature increases (and suction decreases),
- stop if you are sure that the heating process proceeding slowly,
- take a final look and charge when the system is stable and the heat pump stops normally (setpoint is reached), this may take 12 hours or more,
- after the final refilling difference between the suction pressure temperature and the Tae sensor temperature should be 3 ... 6 °C.

This algo is good and safe both the first time and as a starting point. As you gain experience, you will get yours much easier and faster.<br>
Also, use "manual EEV mode" during refilling process.<br>
Keep your eyes protected and do not freeze your fingers.<br>
<img src="./m_add_charge.jpg" height="200"><br><br>

## Hints
For more information about Heat Pumps look at [Wikipedia HP page](https://en.wikipedia.org/wiki/Heat_pump).<br>
If you want to know "how the refrigeration systems work", read Patrick Kotzaoglanian books.<br>
If you want more technical details, sophisticated schemes, "how EEV can be driven by temperature" diagrams, etc. refer to vendor manuals (you'll find all you need in the Alfa Laval brochures, Danfoss guides, and so on).<br>
For refrigerants and oils types comparison see wiki.<br><br>

## Personal experience 
Note that the SCT013 sensor and the current monitoring scheme cannot be used for accurate measurements and accurate COP calculations. Use a watt meter for accurate power measurements.<br>
Temperature sensor installation at a warm floor surface is a bad idea - it's better to get "hot in" water temperature coming from all over the floor, as implemented in firmware.<br>
The weather-dependent (both outdoor and indoor temperature dependent) system does not work fine for 30-150 m2 buildings. Such a system is too complex and works unpredictable due to random ventilation. And also due to the unpredictability of heat emitted in the house by other sources.<br>
I tried the scheme with a flooded evaporator in 2019 and found it terribly tricky, then refused to use it.<br>
Deep regeneration schemes are useful only for some refrigerants and only in certain temperature ranges. I've tried deep regeneration too. As a result, the theory coincided with practice and I also refused this idea.<br>
In general, it is possible by complicating the refrigeration scheme to win somewhere 1%, somewhere 3%, but all this leads to significant time and money cost getting suddenly a small profit.<br>
Summary: If you want experiments - Experiment. Want reliably - make the system simple.<br><br>
<img src="./m_add_freezed1.jpg" height="300"> <img src="./m_add_freezed2.jpg" height="300"> <img src="./m_add_freezed3.jpg" height="300"><br><br>

## Author
<br>
gonzho АТ web.de (c) 2018-2021<br>

## Appendix A: Abbreviations
Abbreviations used in the documentation and firmware.<br>
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

The additional sensor used in "swimming pool heater" or "water tank heater" schemes, check **SETPOINT_TS1** option:
| Abbr. | Full name             |
| ----- | --------------------  |
| Ts1   | additional sensor1    |

Additional sensors, disabled and not used by default:
| Abbr. | Full name             |
| ----- | --------------------  |
| Treg  | regenerator temperature|
| Ts2   | additional sensor2    |

Relays:
| Abbr. | Full name             |
| ----- | --------------------  |
| RCRCH  | crankcase heater relay	|
| RC   | cold side water pump relay	|
| RH   | cold side water pump relay    	|
| RP   | heat pump (compressor) relay	|

Other:
| Abbr. | Full name             		|
| ----- | --------------------  		|
| LSM	| LastStopCause				|
| LSC	| LastStartMessage			|
| CWP/CCP | cold side water (circulating) pump	|
| HWP/HCP | cold side water (circulating) pump	|
| EEVP	| EEV position				|
| HP	| heat pump				|

## Appendix B: LEDs
LEDs allow you to make rapid diagnostics without connecting a serial console or a Service Display.<br>
| LED | description |
| ------------- | ------------- |
| **EEV_opening**     	|  EEV is opening  |
| **EEV_closing**     	|  EEV is closing  |
| **EEV_fast**     	|  EEV mode is "fast" (non-precise) |
| **485_RX**     	|  485 transceiver is in listening state  |
| **485_TX**     	|  485 transceiver transmits a reply  |
| **Manual mode**     	|  EEV in a manual mode   |
| **LSC: error**     	|  Last stop was caused by an error. If you see this LED ON, it's a reason to connect either console or Service Display. Diagnostics  required.   |
| **LSC: protection**	|  Last stop was caused by protection. In some cases (like long powered-on periods or refilling) this can occur. Here no recommendations about diagnostic, since all systems and operation conditions are different. This LED indicates that something exceeded normal run conditions. |
| **OK**     |  System OK.  |
| **ERROR**     |  Something wrong: not all T sensors connected, one of the pressure sensors is not OK. Diagnostics required.   |
| **Relays LEDs**     |  Indicates corresponding relay state  |

## Appendix C: Diagnostic and status messages
**LastStopCause (LSC) messages. Why the compressor has stopped working.**
| Message | description |
| ------------- | ------------- |
| **Normal_stop**     | Normal stop, i.e. setpoint sensor temperature > setpoint, so heat not needed.    |
| **P.WtMax:_WATTS_**    | Protective stop. Overcurrent, double-check your system, then **MAX_WATTS** and **POWERON_HIGHTIME** options. |
| **P.W.wattMIN**    | Protective stop. Abnormally low power consumption (<max watts/5). Check your system, see **MAX_WATTS** option. |
| **P.W.TcrcMIN**    | Protective stop. Abnormally low "Compressor" temperature. Check your system, see **T_WORKINGOK_CRANKCASE_MIN** option. |
| **P.Tho**         | Protective stop. "Hot out" temperature is too high. Check your system, see **T_HOT_MAX** option. |
| **P.Thi**         | Protective stop. "Hot in" temperature is too high. Check system, see **T_HOT_MAX** option. |
| **P.Tcrc**        | Protective stop. "Compressor" temperature is too high. Overheat protection. This is an ordinary situation during long runs. See **T_CRANKCASE_MAX** option and compressor manual if you want to tune it (~115 °C for wide-available compressors). |
| **P.Tae**        | Protective stop. "After evaporator" temperature too low. Preventing cold loop from freezing and protecting suction line from liquid. See **T_COLDREF_MIN** option. |
| **P.Tbe**        | Protective stop. "Before evaporator" temperature too low. Preventing cold loop from freezing. See **T_BEFORE_EVAP_WORK_MIN** option. |
| **P.Tbc**        | Protective stop. "Before condenser" temperature is too high. Overheat protection. This is an ordinary situation during long runs. See **T_BEFORE_CONDENSER_MAX** option. |
| **P.Tci**        | Protective stop. "Cold in" temperature is too low. Preventing cold loop from freezing. See **T_COLD_MIN** option. |
| **P.Tco**        | Protective stop. "Cold out" temperature is too low. Preventing cold loop from freezing. See **T_COLD_MIN** option. |
| **E.Tci, E.Tco, E.Tbe, E.Tae, E.Ts1, E.Ts2, E.Tcrc, E.Treg, E.Tac, E.Tbc, E.Tho, E.Thi** |    Sensor lost, check wiring. Refer to "T sensor abbreviations". |
| **E.PresCold**    | Cold side pressure too low, check refrigerant charge and pressure sensors. |
| **E.PresHot**     | Hot side pressure too high, check refrigerant charge and pressure sensors. |

**LastStartMessage (LSM) messages. What condition the system expects. Some informational messages.**
| Message | description |
| ------------- | ------------- |
| **StCntd:_seconds_** 	| Startup countdown, short-term power loss protection. |
| **HP_Started** 	| Normal start. |
| **#Thi>Setp.** 	| "Hot in" temperature > setpoint, so no reason to start. |
| **#Ts1>Setp.** 	| "Ts1" temperature > setpoint, so no reason to start. See **SETPOINT_TS1** option to switch between Thi and Ts1 as setpoint sensor. |
| **HWP_OFF** 		| Setpoint sensor temperature > setpoint, so after some time (**HOTCIRCLE_STOP_AFTER** option) hot side pump powered off and gone to power saving mode. |
| **HWP_ON_by_ev** 	| Hot side pump started after power saving. See **HOTCIRCLE_START_EVERY** option. |
| **#HotPrp:_seconds_** | Hot side pump is on, waiting for T stabilization. Countdown, seconds. See **HOTCIRCLE_CHECK_PREPARE** option. |
| **#HotSlp:_seconds_** | Hot side pump in power save mode (sleeping). Waiting for next startup. Countdown, seconds. See **HOTCIRCLE_START_EVERY** option. |
| **#HPSlp:_seconds_**	| Compressor: pause between starts. Countdown, seconds. **MINCYCLE_POWEROFF** option.|
| **#CPpStart** 	| Cold side pump started. |
| **#CPp:_seconds_** 	| Cold side pumping. Preparing the system to compressor start. Countdown, seconds. **COLDCIRCLE_PREPARE** option. |
| **#Tho>Max** 		| "Hot out" temperature is too high. See **T_HOT_MAX** option. |
| **#Thi>Max** 		| "Hot in" temperature is too high. See **T_HOT_MAX** option. |
| **#CaseCold**		| Compressor crankcase temperature is too low. The system can't start. This situation occurs on outdoor installations during a winter season and if AC power lost for a few hours. Wait, while the crankcase heater stabilizing your compressor temperature. See **T_CRANKCASE_MIN** option. |
| **#CaseHot**        | Compressor is still overheated, waiting. See **T_CRANKCASE_MAX** option. |
| **#Tae<RefMin**    | "After evaporator" temperature too low. Preventing cold loop from freezing and protecting suction line from liquid. See **T_COLDREF_MIN** option. |
| **#Tbe<RefMin**    | "Before evaporator" temperature too low. Preventing cold loop from freezing. See **T_COLDREF_MIN** option. |
| **#Tbc>Max**        | "Before condenser" temperature is too high. Overheat protection. See **T_BEFORE_CONDENSER_MAX** option. |
| **#Tci<ColdMin**    | "Cold in" temperature is too low. Preventing cold loop from freezing. See **T_COLD_MIN** option. |
| **#Tco<ColdMin**    | "Cold out" temperature is too low. Preventing cold loop from freezing. See **T_COLD_MIN** option. |
| **CWP_ON_CoMin**    | Cold side pump started because cold side temperature is too low, so preventing cold loop freeze, see **T_COLD_MIN** option. |

**Additional messages**
| Message | description |
| ------------- | ------------- |
| **OK:Pr.Cold**	| Cold side pressure restored. |
| **OK:Pr.Hot**		| Hot side pressure restored.  |
| **OK:E.T.Sens.**	| Temperature sensors restored.|
| **HWP_ON** 		| Hot side pump powered on.    |
| **Err:_errorcode_**	| Error code: 1 = temperature sensor error, 2 = Hot side pressure too high, 3 = cold side pressure too low. |

##  Appendix D: secret appendix
Are you still reading? It seems you are interested in Heat Pumps, so this appendix is for you.<br>
About sensors: avoid using cheap "waterproof epoxy-covered" sensors. "Waterproof" lasts for a short time.<br>
Buy DS18B20s chips. No matter what sensors are buying: cheap or at a high price. I've never seen "bad" DSes. Solder sensors to the wires and cover with two layers of 2-component epoxy resin as pictured below. It will work for years. White/orange - GND, white/blue - signal, orange - +5V.<br>
<img src="./m_ds18b20_epoxy.jpg" width="500"><br><br>
For sensors at your compressor and discharge (+100 °C and higher) use heat-resistant sleeves at every wire.<br>
<img src="./m_ds18b20_wires_protection.jpg" width="500"><br><br>
To get precise temperature readings protect sensors against ambient air temperature influence with additional thermal insulation. Temperature readings from most of the sensors are interesting, but +/- few degrees does not matter. So, cover most of the sensors with thermal insulation as you wish.<br>
But two sensors "Before evaporator" and "After evaporator" are critical to EEV and needs an extra attention. The temperature of these sensors must be as close to the temperature of the copper tube as it possible. So, install Tae and Tbe sensors as pictured below. You can use thermal paste, but it is no significant difference with much more available silicone. Tape not shown at photos below, for clarity, but should be used with every insulation layer.<br>
<img src="./m_ds18b20_evaporator_mount.jpg" height="700"><br><br>
About water(glycol)<->refrigerant heat exchangers. You can use plate heat exchangers. Pros: the best efficiency. Cons: costs money. Potential oil return difficulties.<br>
<img src="./m_plate_heat_exchangers.jpg" width="500"><br><br>
And oxygen brazing with (20%+)silver+copper solder required here:<br>
<img src="./m_plate_echangers_oxygen_brazing.jpg" width="500"><br><br>
You can build your own "tube-in-tube" heat exchangers. It's not hard. Cheaper. The heat exchange efficiency is worse. No oil return problems. Very easy soldering. Heat exchanger math: 0.7..1.5 m<sup>2</sup> of a copper tube per every 3kW of heat transfer.<br>
<img src="./m_tube-in-tube_diy1.jpg" width="400"> <img src="./m_tube-in-tube_diy2.jpg" width="400"><br><br>
Additionally, you can think "I'll take an old AC parts... Housing... Slightly change... An hour or two, day of work maximum and I'll get a refrigerant<->water heat exchanger in for a penny!". This idea is obvious. It was the first thing I've tried. You can try this, but to achieve "not very bad" performance it'll take more than a one day and much more than a few $$, even if you have unlimited access to older ACs.<br>
Ok, I think that's enough for this appendix, this is a controller page, and not how-to-build-refrigeration-systems page.<br>
Overall, your system with sensor locations will look like at a scheme below. Refrigerators (heat pumps) are simple devices.<br>
<img src="./m_Valden_Heat_Pump_Controller_model.jpg" width="1000"><br><br>
Your system works (or sleeps) depending on Thi temperature. For the end user it looks like setting up comfortable temperature of the warm floor via Remote Display.

## Appendix E: Firmware options and fine-tuning

QA tests, uncomment to enable
```c
//#define SELFTEST_RELAYS_LEDS_SPEAKER 	//speaker and relays QA test, uncomment to enable
//#define SELFTEST_EEV 			//EEV QA test, uncomment to enable
//#define SELFTEST_T_SENSORS 		//temperature sensors QA test, uncomment to enable
```

Communication protocol with an external world. Choose one

```c
//#define RS485_JSON 1 		//JSON, external systems integration
//#define RS485_HUMAN 2 	//RS485 is used in the same way as the local console, warning: Use only if 2 devices (server and this controller) connected to the same RS485 line
#define RS485_MODBUS 3 		//default, MODBUS via RS485, connection to the display (both sensor or 1602, see https://GitHub.com/OpenHP/Display/) or connection to any other MODBUS application or device 
```

System type, comment both if HP with EEV
```c
//#define EEV_ONLY 	//Valden controller as EEV controller: NO target T sensor. No relays. Oly EEV. Sensors required: Tae, Tbe, current sensor. Additional T sensors can be used but not required.
//#define NO_EEV 	//capillary tube or TXV, EEV not used
```

Sensor used to check setpoint, uncomment one of those options
```c
#define SETPOINT_THI 	//"warm floor" scheme: "hot in" (Thi) temperature used as setpoint
//#define SETPOINT_TS1 	//"swimming pool" or "water tank heater" scheme: "sensor 1" (Ts1) is used as setpoint and located somewhere in a water tank
```

Some more options
```c
#define HUMAN_AUTOINFO	30000	//print stats to console, every milliseconds
#define WATCHDOG		//disable for older bootloaders
```

<b>Next sections: advanced options</b><br>
<img src="./m_add_graph.png" width="826"><br>
Temperature sensors used in a system, comment to disable 

```c
#define T_cold_in;		//cold side (heat source) inlet sensor
#define T_cold_out;		//cold side outlet sensor
#define T_before_evaporator;	//"before" and "after evaporator" sensors required to control EEV, both "EEV_ONLY" and "full" schemes 
#define T_after_evaporator;	//"before" and "after evaporator" sensors required to control EEV, both "EEV_ONLY" and "full" schemes 
#ifdef SETPOINT_TS1
	#define T_sensor_1;	//T values from the additional sensor S1 used as a "setpoint" in "pool" or "water tank heater" schemes 
#endif
//#define T_sensor_2;		//additional sensor, any source; for example, outdoor temperature, in-case temperature, and so on
#define T_crc;			//if defined, enables the crankcase T sensor and crankcase heater on the relay "Crankcase heater"
//#define T_regenerator;	//an additional sensor, the regenerator temperature sensor (inlet or outlet or housing); used only to obtain a temperature data if necessary 
#define T_afrer_condenser;	//after condenser (and before valve)
#define T_before_condenser;	//before condenser (discharge)
#define T_hot_out;		//hot side outlet
//In full scheme Hot IN required! Optional in "EEV_ONLY" scheme (see "EEV_ONLY" option), 
#define T_hot_in;		//hot side inlet
```

Temperature limits
```c
#define MAGIC		0x66;	//change this value if you want to rewrite the T setpoint in EEPROM 
#define	T_SETPOINT	26.0;	//This is a predefined target temperature value (start temperature). EEPROM-saved. Ways to change this value: 1. Console command 2. Change the "setpoint" on a display 3. Change value here AND change "magic number" 4. JSON command
#define T_SETPOINT_MAX	48.0;	//maximum "setpoint" temperature that an ordinary user can set
#define T_SETPOINT_MIN	10.0;	//min. "setpoint" temperature that an ordinary user can set, lower values not recommended until antifreeze fluids at hot side used.
#define T_CRANKCASE_MIN		8.0;	//compressor (crankcase) min. temperature, HP will not start if T lower
#define T_CRANKCASE_MAX		110.0;	//compressor (crankcase) max. temperature, overheating protection, HP will stop if T higher
#define T_CRANKCASE_HEAT_THRESHOLD 16.0;//crankcase heater threshold, the compressor heater will be powered on if T lower
#define T_WORKINGOK_CRANKCASE_MIN  25.0;//compressor temperature: additional check. HP will stop if T is lower than this value after 5 minutes of work. Do not set the value too high to ensure normal operation after long pauses. 
#define T_BEFORE_CONDENSER_MAX	108.0;	//discharge MAX, system stops if discharge higher
#define T_COLDREF_MIN 		-14.0;	//suction min., HP stops if T lower, cold side (glycol) loop freeze protection and compressor protection against liquid 
#define T_BEFORE_EVAP_WORK_MIN 	-25.5;	//!!!before evaporator (after valve) min. T; can be very low for a few minutes after a startup, ex: capillary tube in some conditions; and for all systems: after long shut-off, lack of refrigerant, 1st starts, and many others
#define T_COLD_MIN 		-15.5;	//cold side (glycol) loop freeze protection: HP stops if inlet or outlet temperature lower
#define T_HOT_MAX 		50.0;	//hot loop: HP stops if hot side inlet or outlet temperature higher than this threshold
```

Watts, cycles times (milliseconds)
```c
#define MAX_WATTS	1000.0 + 70.0 + 80.0	//power limit, watt, HP stops if exceeded, example: compressor: ~1000 + hot CP 70 + cold CP 80
#define POWERON_PAUSE     	300000    //after power on: 			wait 5 minutes before starting HP (power faults protection) 
#define MINCYCLE_POWEROFF 	600000    //after a normal compressor stop: 	10 minutes pause (max 99999 seconds) 
#define MINCYCLE_POWERON  	3600000  //after compressor start: 		minimum compressor operation time, i.e. work time is not less than this value (or more, depending on the setpoint temperature) 60 minutes = 3.6 KK 120mins = 5.4 kK.
#define POWERON_HIGHTIME	7000	//after compressor start: 		defines time when power consumption can be 3 times greater than normal, 7 sec. by default
#define COLDCIRCLE_PREPARE	90000	//before compressor start:		power on cold CP and wait 90 sec.; if false start: CP will off twice this time; and (hotcircle_stop_after - this_value) must be > hotcircle_check_prepare or HP will go sleep cycle instead of start
#define DEFFERED_STOP_HOTCIRCLE	1200000	//after compressor stop:		wait 20 minutes, if no need to start compressor: stop hot WP; value must be > 0
#define HOTCIRCLE_START_EVERY	2400000	//while pauses:				pump on "hot side"  starts every 40 minutes (by default) (max 9999 seconds) to circulate water and get exact temperature reading, option used if "warm floor" installation (Thi as setpoint)...
#define HOTCIRCLE_CHECK_PREPARE	150000	//while pauses:				...and wait for temperature stabilization 2.5 minutes (by default), after that do setpoint checks...
#define HOTCIRCLE_STOP_AFTER	(HOTCIRCLE_CHECK_PREPARE + COLDCIRCLE_PREPARE + 30000)		//...and then stop after few minutes of circulating, if temperature is high and no need to start compressor; value must be check_prepare + coldcircle_prepare + 30 seconds (or more)
```

EEV options.<br>
<img src="./m_add_eev.jpg" height="200"><br>
If you are using a capillary tube or TXV: simply skip next section.<br>
Depending on how many milliseconds allocated per step, the speed of automatic tuning will change.<br>
Remember that your refrigeration system reaction on every step is not immediate. The system reacts after a few minutes, sometimes after tens of minutes.<br>
```c
#define EEV_MAXPULSES		250	//max steps, 250 is tested for sanhua 1.3

//steps tuning: milliseconds per fast and slow (precise) steps
#define EEV_PULSE_FCLOSE_MILLIS	20	//fast closing, closing on danger			(milliseconds per step)
#define EEV_PULSE_CLOSE_MILLIS	60000	//accurate closing while the compressor works 		(milliseconds per step)
#define EEV_PULSE_WOPEN_MILLIS	20	//standby (waiting) pos. set				(milliseconds per step)
#define EEV_PULSE_FOPEN_MILLIS	1400	//fast opening, fast search 				(milliseconds per step)
#define EEV_PULSE_OPEN_MILLIS	70000	//accurate opening while the compressor works		(milliseconds per step)
#define EEV_STOP_HOLD		500	//0.1..1sec for Sanhua		hold time		(milliseconds per step)
#define EEV_CLOSEEVERY		86400000	//86400000: EEV full close (zero calibration) every 24 hours, executed while HP is NOT working	(milliseconds per cycle)

//positions
#define EEV_CLOSE_ADD_PULSES	8	//read below, additional steps after zero position while full closing 
#define EEV_OPEN_AFTER_CLOSE	45	//0 - set the zero position, then add EEV_CLOSE_ADD_PULSES (zero insurance, read EEV guides for this value) and stop, EEV will be in zero position. 
					//N - set the zero position, then add EEV_CLOSE_ADD_PULSES, than open EEV on EEV_OPEN_AFTER_CLOSE pulses
					//i.e. it's a "waiting position" while HP isn't working, value must be <= MINWORKPOS
#define EEV_MINWORKPOS		50	//position will be not less during normal work, open EEV to this position after compressor start

//temperatures
#define EEV_PRECISE_START	7.0	//precise tuning threshold: 		make slower pulses if (real_diff-target_diff) less than this value. Used for fine auto-tuning
#define EEV_EMERG_DIFF		1.7	//liquid at suction threshold:		if dangerous condition occurred, real_diff =< (target_diff - EEV_EMERG_DIFF)  then EEV will be closed to min. work position //Ex: EEV_EMERG_DIFF = 2.0, target diff 5.0, if real_diff =< (5.0 - 2.0) then EEV will be closed to EEV_MINWORKPOS
#define EEV_HYSTERESIS		0.5	//hysteresis, to stop fine-tuning:	must be less than EEV_PRECISE_START, ex: target difference = 4.0, hysteresis = 0.3, no EEV pulses will be done while real difference in range 4.0..4.3 
#define EEV_TARGET_TEMP_DIFF	3.6	//target difference between Before Evaporator and After Evaporator, the head of the whole algorithm

//additional options
#define EEV_REOPENLAST		1	//1 = reopen to last position on compressor start, useful for ordinary schemes with everyday working cycles, 0 = not
#define EEV_REOPENMINTIME	40000	//after system start: min. delay between "min. work pos." (must be > 0 in this case and > waiting position) set and reopening start
//#define EEV_MANUAL			//comment to disable, manual set of EEV position via a console; warning: this option will stop all EEV auto-activities, including zero position find procedure; so this option not recommended: switch auto/manual mode from a console

//do not use next option if you're not sure what are you doing
//#define EEV_DEBUG				//debug, useful during system fine-tuning, works both with local serial and RS485_HUMAN
```

Communication addresses
```c
const char devID  = 0x45;	//used only if JSON communication, does not matter for MODBUS and Valden display https://github.com/OpenHP/Display/
const char hostID = 0x30;	//used only if JSON communication, not used for MODBUS
```

Last option
```c
#define MAX_SEQUENTIAL_ERRORS 	15 		//max cycles to wait auto-clean error, ex: T sensor appears, stop compressor after counter exceeded (millis_cycle * MAX_SEQUENTIAL_ERRORS)
```
## Appendix D: bill of materials
<img src="./m_add_parts.jpg"><br>
| Part | Quantity |
| ------------- | ------------- |
| **1206 Resistors:**	||
| 10	| 1	|
| 100	| 1	|
| 120	| 1	|
| 1K	| 7	|
| 10K	| 6	|
| 100K	| 2	|
| 22	| 1	|
| 2.2K	| 4	|
| 470	| 10	|
| **1206 Caps:**	||
| 0.01uF	| 2	|
| 0.1uF		| 4	|
| 1uF		| 8	|
| 10uF		| 5	|
| **1206 LEDs:**	||
| Red (error LEDs)	| 2	|
| Green (OK LED)	| 1	|
| Yellow 		| 11	|
| **SOP(SOIC) ICs:**	|	|
| ADM2587EBRWZ (SOIC-20)	| 1	|
| 74HC4067D (SOIC-24)		| 1	|
| 74HC595D (SOP-16)		| 3	|
| ULN2003A_(SOP-16)		| 2	|
| 817S (SOP-4)			| 2	|
| **XH2.54 Headers + Plugs:**	|	|
| XH2.54-6P header + 6P plug	| 4	|
| XH2.54-3P header + 3P plugs	| 2	|
| XH2.54-2P header + 2P Plugs	| 1	|
| XH2.54 Crimp terminal		| 40	|
| **Power terminals:**		|	|
| 6.35 Blade terminal (726386-2 or same)		| 16	|
| 6.35 Quick disconnect crimp terminal insulated	| 16	|
| **Others:**	|				|	
| 10nF HV-9.0x3.0 (blue disc 2kV HV cap)	| 1	|
| 22uf_16v D5.0xF2.0 (electrolytic cap)		| 1	|
| Resistor Network\*4 DIP-1X5P-2.54 (3..5K)	| 3	|
| MMBT2222A (SOT-23-3)				| 1	|
| LM7805 (TO-220)				| 1	|
| SMIH-12VDC-SL-C				| 4	|
| BUZZER-R9.0-2P-4.0				| 1	|
| ARDUINO PRO MINI				| 1	|
| Power supply, 12v1.25A 70x30x40 (or any 0.5A+)| 1	|
| DS18B20					| 12	|
| USB<->UART (to upload firmware)		| 1	|
| Current sensor SCT-013-000			| 1	|

## License
© 2018-2021 D.A.A. All rights reserved; gonzho AT web.de; https://github.com/openhp/HeatPumpController/.<br>

Text, media and other materials licensed under [CC-BY-SA License v4.0](https://creativecommons.org/licenses/by-sa/4.0/).<br>
<sub>Attribution: You must clearly attribute Valden Heat Pump Controller (https://github.com/openhp/HeatPumpController/) original work in any derivative works.<br>
Share and Share Alike: If you make modifications or additions to the content you re-use, you must license them under the CC-BY-SA License v4.0 or later.<br>
Indicate changes: If you make modifications or additions, you must indicate in a reasonable fashion that the original work has been modified.<br>
You are free: to share and adapt the material for any purpose, even commercially, as long as you follow the license terms.</sub><br>

The firmware source code licensed under [GPLv3](https://www.gnu.org/licenses/gpl-3.0.en.html). <br>
<sub>This product is distributed in the hope that it will be useful,	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.</sub><br>

For third-party libraries licenses used in this product please refer to those libraries.<br>
## Author
<br>
gonzho АТ web.de (c) 2018-2021<br>
