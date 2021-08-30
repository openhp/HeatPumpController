/*
	
	Valden Heat Pump.
	
	Heat Pump Controller firmware.
	
	https://github.com/OpenHP/
	
	Copyright (C) 2018-2021 gonzho@web.de
	
	

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


//-----------------------USER OPTIONS-----------------------
//#define SELFTEST_RELAYS_LEDS_SPEAKER		//speaker and relays QA test, uncomment to enable
//#define SELFTEST_EEV				//EEV QA test, uncomment to enable
//#define SELFTEST_T_SENSORS			//temperature sensors QA test, uncomment to enable

//communication protocol with external world
//#define RS485_JSON		1  		//json, external systems integration
//#define RS485_HUMAN   	2		//RS485 is used in the same way as the local console, warning: Use only if 2 devices (server and this controller) connected to the same RS485 line
#define RS485_MODBUS 		3 		//default, MODBUS via RS485, connection to the display (both sensor or 1602, see https://GitHub.com/OpenHP/Display/) or connection to any other MODBUS application or device 

//system type, comment both if HP with EEV
//#define	EEV_ONLY			//Valden controller as EEV controller: NO target T sensor. No relays. Oly EEV. Sensors required: Tae, Tbe, current sensor. Additional T sensors can be used but not required.
//#define	NO_EEV				//capillary tube or TXV, EEV not used

//which sensor is used to check setpoint, uncomment one of options
#define SETPOINT_THI				//"warm floor" scheme: "hot in" (Thi) temperature used as setpoint
//#define SETPOINT_TS1				//"swimming pool" or "water tank heater" scheme: "sensor 1" (Ts1) is used as setpoint and located somewhere in a water tank

#define HUMAN_AUTOINFO	30000			//print stats to console, every milliseconds

#define WATCHDOG          			//disable for older bootloaders
//-----------------------USER OPTIONS END-----------------------



//-----------------------Fine Tuning OPTIONS-----------------------
//next sections: advanced options



//-----------------------T Sensors -----------------------
//temperature sensors used in a system, comment to disable 
#define T_cold_in;			//cold side (heat source) inlet sensor
#define T_cold_out;			//cold side outlet sensor
#define T_before_evaporator;		//"before" and "after evaporator" sensors required to control EEV, both "EEV_ONLY" and "full" schemes  
#define T_after_evaporator;		//"before" and "after evaporator" sensors required to control EEV, both "EEV_ONLY" and "full" schemes 
//#define T_separator_gas;		//no longer used (PCB 1.3 MI +) artifact from experimental scheme with separator 
//#define T_separator_liquid;		//no longer used (PCB 1.3 MI +) artifact from experimental scheme with separator 
//#define T_before_valve;		//no longer used (PCB 1.3 MI +) artifact from experimental scheme with separator 
//#define T_suction;			//no longer used (PCB 1.3 MI +) artifact from experimental scheme with separator 
#ifdef SETPOINT_TS1
	#define T_sensor_1;		//T values from the additional sensor S1 used as a "setpoint" in "pool" or "water tank heater" schemes  
#endif
//!!!
#define T_sensor_2;			//additional sensor, any source; for example, outdoor temperature, in-case temperature, and so on
#define T_crc;				//if defined, enables the crankcase T sensor and crankcase heater on the relay "Crankcase heater"
//#define T_regenerator;		//an additional sensor, the regenerator temperature sensor (inlet or outlet or housing); used only to obtain a temperature data if necessary 
#define T_afrer_condenser;		//after condenser (and before valve)
//!!!#define T_before_condenser;	//before condenser (discharge)
#define T_hot_out;			//hot side outlet
//In full scheme Hot IN required! Optional in "EEV_ONLY" scheme (see "EEV_ONLY" option), 
#define T_hot_in;			//hot side inlet

//-----------------------TEMPERATURES-----------------------
#define MAGIC     		0x66;  		//change this value if you want to rewrite the T setpoint in EEPROM 
#define	T_SETPOINT		26.0;		//This is a predefined target temperature value (start temperature). EEPROM-saved. Ways to change this value: 1. Console command 2. Change the "setpoint" on a display 3. Change value here AND change "magic number" 4. JSON command
#define T_SETPOINT_MAX 		48.0;  		//maximum "setpoint" temperature that an ordinary user can set
#define T_SETPOINT_MIN		10.0;		//min. "setpoint" temperature that an ordinary user can set, lower values not recommended until antifreeze fluids at hot side used.
#define T_CRANKCASE_MIN 		8.0;	//compressor (crankcase) min. temperature, HP will not start if T lower
#define T_CRANKCASE_MAX 		110.0;	//compressor (crankcase) max. temperature, overheating protection, HP will stop if T higher
#define T_CRANKCASE_HEAT_THRESHOLD 	16.0;	//crankcase heater threshold, the compressor heater will be powered on if T lower
#define T_WORKINGOK_CRANKCASE_MIN 	25.0;	//compressor temperature: additional check. HP will stop if T is lower than this value after 5 minutes of work. Do not set the value too high to ensure normal operation after long pauses. 
#define T_BEFORE_CONDENSER_MAX 	108.0;      	//discharge MAX, system stops if discharge higher
#define T_COLDREF_MIN 		-14.0;		//suction min., HP stops if T lower, cold side (glycol) loop freeze protection and compressor protection against liquid 
#define T_BEFORE_EVAP_WORK_MIN 	-25.5;		//!!!before evaporator (after valve) min. T; can be very low for a few minutes after a startup, ex: capillary tube in some conditions; and for all systems: after long shut-off, lack of refrigerant, 1st starts, and many others
#define T_COLD_MIN 		-15.5;		//cold side (glycol) loop freeze protection: HP stops if inlet or outlet temperature lower
#define T_HOT_MAX 		50.0;		//hot loop: HP stops if hot side inlet or outlet temperature higher than this threshold

//#define T_REG_HEAT_THRESHOLD	17.0;		//no longer used (PCB 1.3 MI +) artifact from experimental scheme with separator 
//#define T_HOTCIRCLE_DELTA_MIN 	2.0;	//not used since ~FW v1.6, "water heater with intermediate heat exchanger" scheme, where Ts1 == "sensor in water"; hot side CP will be switched on if "Ts1 - hot_out > T_HOTCIRCLE_DELTA_MIN"

//-----------------------WATTS AND CYCLES TIMES-----------------------
//time: milliseconds, power: watts
#define MAX_WATTS	1000.0 + 70.0 + 80.0	//power limit, watt, HP stops if exceeded, examples: 	//	installation1: compressor 165: 920 Watts, + 35 watts 25/4 circ. pump at 1st speed + 53 watts 25/4 circ. pump at 2nd speed
													//	installation2: compressor unk: ~1000 + hot CP 70 + cold CP 80 = 1150 watts
													//	installation3: and so on
#define POWERON_PAUSE     	300000    	//after power on: 			wait 5 minutes before starting HP (power faults protection) 
#define MINCYCLE_POWEROFF 	600000    	//after a normal compressor stop: 	10 minutes pause (max 99999 seconds) 
#define MINCYCLE_POWERON  	3600000  	//after compressor start: 		minimum compressor operation time, i.e. work time is not less than this value (or more, depending on the setpoint temperature) 60 minutes = 3.6 KK 120mins = 5.4 kK.
#define POWERON_HIGHTIME	7000		//after compressor start: 		defines time when power consumption can be 3 times greater than normal, 7 sec. by default
#define COLDCIRCLE_PREPARE	90000		//before compressor start:		power on cold CP and wait 90 sec.; if false start: CP will off twice this time; and (hotcircle_stop_after - this_value) must be > hotcircle_check_prepare or HP will go sleep cycle instead of start
#define DEFFERED_STOP_HOTCIRCLE	1200000		//after compressor stop:		wait 20 minutes, if no need to start compressor: stop hot WP; value must be > 0
#define HOTCIRCLE_START_EVERY	2400000		//while pauses:				pump on "hot side"  starts every 40 minutes (by default) (max 9999 seconds) to circulate water and get exact temperature reading, option used if "warm floor" installation (Thi as setpoint)...
#define HOTCIRCLE_CHECK_PREPARE	150000		//while pauses:				...and wait for temperature stabilization 2.5 minutes (by default), after that do setpoint checks...
#define HOTCIRCLE_STOP_AFTER	(HOTCIRCLE_CHECK_PREPARE + COLDCIRCLE_PREPARE + 30000)		//...and then stop after few minutes of circulating, if temperature is high and no need to start compressor; value must be check_prepare + coldcircle_prepare + 30 seconds (or more)


//-----------------------EEV-----------------------
//If you are using a capillary tube or TXV: simply skip next section.
//Depending on how many milliseconds allocated per step, the speed of automatic tuning will change.
//Remember that your refrigeration system reaction on every step is not immediate. The system reacts after a few minutes, sometimes after tens of minutes.

#define EEV_MAXPULSES		250		//max steps, 250 is tested for sanhua 1.3

//steps tuning: milliseconds per fast and slow (precise) steps
#define EEV_PULSE_FCLOSE_MILLIS	20		//(20 tube evaporator)		fast closing, closing on danger			(milliseconds per step)
#define EEV_PULSE_CLOSE_MILLIS	45000		//(50000 tube evaporator)	accurate closing while the compressor works 	(milliseconds per step)
#define EEV_PULSE_WOPEN_MILLIS	20		//(20 tube evaporator)		standby (waiting) pos. set			(milliseconds per step)
#define EEV_PULSE_FOPEN_MILLIS	1400		//(1300 tube evaporator)	fast opening, fast search 			(milliseconds per step)
#define EEV_PULSE_OPEN_MILLIS	30000		//(60000 tube evaporator)	accurate opening while the compressor works	(milliseconds per step)
#define EEV_STOP_HOLD		500		//0.1..1sec for Sanhua		hold time					(milliseconds per step)
#define EEV_CLOSEEVERY		86400000	//86400000: EEV full close (zero calibration) every 24 hours, executed while HP is NOT working	(milliseconds per cycle)

//positions
#define EEV_CLOSE_ADD_PULSES	8		//read below, additional steps after zero position while full closing 
#define EEV_OPEN_AFTER_CLOSE	45		//0 - set the zero position, then add EEV_CLOSE_ADD_PULSES (zero insurance, read EEV guides for this value) and stop, EEV will be in zero position. 
						//N - set the zero position, then add EEV_CLOSE_ADD_PULSES, than open EEV on EEV_OPEN_AFTER_CLOSE pulses
						//i.e. it's a "waiting position" while HP isn't working, value must be <= MINWORKPOS
#define EEV_MINWORKPOS		50		//position will be not less during normal work, open EEV to this position after compressor start

//temperatures
#define EEV_PRECISE_START	8.6		//(8.6 tube evaporator) 	precise tuning threshold: 		make slower pulses if (real_diff-target_diff) less than this value. Used for fine auto-tuning
#define EEV_EMERG_DIFF		1.7		//(2.5 tube evaporator) 	liquid at suction threshold:		if dangerous condition occurred, real_diff =< (target_diff - EEV_EMERG_DIFF)  then EEV will be closed to min. work position //Ex: EEV_EMERG_DIFF = 2.0, target diff 5.0, if real_diff =< (5.0 - 2.0) then EEV will be closed to EEV_MINWORKPOS
#define EEV_HYSTERESIS		0.45		//(0.6 tube evaporator) 	hysteresis, to stop fine tuning:	must be less than EEV_PRECISE_START, ex: target difference = 4.0, hysteresis = 0.3, no EEV pulses will be done while real difference in range 4.0..4.3 
#define EEV_TARGET_TEMP_DIFF	3.6		//(3.6 tube evaporator) 	target difference between Before Evaporator and After Evaporator, the head of the whole algorithm

//additional options
#define EEV_REOPENLAST		1		///1 = reopen to last position on compressor start, useful for ordinary schemes with everyday working cycles, 0 = not
#define EEV_REOPENMINTIME	40000		//after system start: min. delay between "min. work pos." (must be > 0 in this case and > waiting position) set and reopening start
//#define EEV_MANUAL				//comment to disable, manual set of EEV position via a console; warning: this option will stop all EEV auto-activities, including zero position find procedure; so this option not recommended: switch auto/manual mode from a console

//do not use next option if you're not sure what are you doing
//#define EEV_DEBUG				//debug, useful during system fine-tuning, works both with local serial and RS485_HUMAN
//-----------------------ADDRESSES-----------------------
const char devID  = 0x45;	//used only if JSON communication, does not matter for MODBUS and Valden display https://github.com/OpenHP/Display/
const char hostID = 0x30;	//used only if JSON communication, not used for MODBUS

//-----------------------OTHER-----------------------
#define MAX_SEQUENTIAL_ERRORS 	15 		//max cycles to wait auto-clean error, ex: T sensor appears, stop compressor after counter exceeded (millis_cycle * MAX_SEQUENTIAL_ERRORS)
//-----------------------Fine Tuning OPTIONS END -----------------------

//-----------------------changelog-----------------------
/*
v1.0, 01 Sep 2019:
+ initial version, hardware and software branch ready

v1.1: 21 Sep 2019:
+ Dev and Host ID to header

v1.2: 20 Dec 2019:
+- ?seems to be fixed minor bug while HP stopped: wattage is 0, if tCrc < T_CRANKCASE_HEAT_THRESHOLD and may be few sensors absence
+ min_user_t/max_user_t to header

v1.3: 05 Jan 2020:
+ manual EEV mode (high priority, ex: new system 1st starts and charge) 
+ rs485_modbus
+ reopen to last EEV value at startup

v1.4: 22 Jan 2020
+ crankcase naming

v1.5: 05 Jun 2020
+ minor modbus updates

v1.6: 09 Dec 2020
+ NO_EEV option
+ some variables renames
+ Tho instead of Thi (stop conditions) bugfix
+ Last Start Message added

v1.7: 03 Feb 2021
+ 1.3 PCB revision support, previous revisions also supported
+ enable cold circle if tci < col_min (circulate ground loop, if outdoor installation and very cold and deep freeze)
+ inputs support
+ add option "Thi" and "Ts1" to header, enable Ts1 by this option
+ temperature check after start of hot side circle + 5 mins for Thi target

v1.8: 06 Feb 2021
+ very rare case: 0.0 readings, 2-3 attempts then pass 0.0
+ countdown for compressor relay after cold CP start (stab. cold loop T)
+ self-test options to header

v1.9-1.11: 25-27 Feb 2021:
+ lot of small workflow logic and user terminal changes

v1.12: 21 Mar 2021:
+ TS1/THO #define way fix
+ CWP and HWP prepare optimisation

v1.13: 26 Mar 2021:
+ rounding error via Modbus found and fixed

//TODO:
? lower bit resolution for all sensors, except Tbe, Tae, Ts1 ?
? poss. DoS: infinite read to nowhere, fix it, set finite counter (ex: 200)
? add "heater start" and "cold circle start" and "not start HP" if t_crc < t_coldin/coldout(?)/tae/tbe(?) + 2.0
? ref. migration protection for summer season with long waiting periods: start cold circle and crankcase heater if tCrc =< tci+1, add option to header
? EEV manual mode and position by RS485 python or modbus command ?
? add speaker and err code for ""ERR: no Tae or Tbe for EEV!""
? deffered HWP stop: check HP stop cause, stop HWP if protective/error stop
? wclose and fclose to EEV
? valve_4way
? rewite re-init proc from MAGIC to another way
? EEV: target to EEPROM (?? no need ?)
? EEV: define maximum working position
*/
//-----------------------changelog END-----------------------

// DS18B20 pins: GND DATA VDD

//Connections:
//DS18B20 Pinout (Left to Right, pins down, flat side toward you)
//- Left   = Ground
//- Center = Signal (Pin N of arduino):  (with 3.3K to 4.7K resistor to +5 or 3.3 )
//- Right  = +5 or +3.3 V   
//

//Speaker
//
// high volume scheme:        +---- +5V (12V not tested)
//                            |
//                       +----+
//                    1MOhm   piezo
//                       +----+
//                            |(C)
// pin -> 1.6 kOhms -> (B) 2n2222        < front here
//                            |(E)
//                            +--- GND
//

/*
scheme SCT-013-000:

2 pins used: tip and sleeve, center (ring) not used http://cms.35g.tw/coding/wp-content/uploads/2014/09/SCT-013-000_UNO-1.jpg
pins are interchangeable due to AC

32 Ohms (22+10) between sensor pins  (35 == ideal)

Pin1: 
- via elect. cap. to GND
- via ~10K..470K resistor to GND
- via ~10K..470K resistor to +5 (same as prev.)
if 10K+10K used: current is 25mA
use 100K+100K for 3 phases

Pin2:
- to analog pin
- via 32..35 Ohms resistor to Pin1

+5 -------------------------+
                            |                  
                            |                                            
                            # R1 10K+                        
                            |                                
                            |                                
                            |~2.5 at this point              
            +---------------+--------------------------------------+----+
            |               |                                      |    |
            #_ elect. cap.  # R2 10K+ (same as R1)     SCT-013-000 $    # R3 = 35 Ohms (ideal case), 32 used  
            |               |                                      |    |
GND --------+---------------+                                      +----+--------> to Analog pin


WARNING: calibrate 3 sensors together, from different sellers, due to case of incorrectly worked 1 of 3 sensor

P(watts)=220*220/R(Ohms)
*/

//
//MAX 485 voltage - 5V
//
// use resistor at RS-485 GND
// 1st test: 10k result lot of issues
// 2nd test: 1k, issues
// 3rd test: 100, see discussions


//16-ch Multiplexer EN pin: active LOW, connect to GND


/*
relay 1: heat pump
relay 2: hot side circulator pump
relay 3: cold side circulator pump
relay 4: crankcase heater
relay 5: (1.3+: not used anymore)

relay 6: reserved
relay 7: reserved

T sensors: 

0 cold_in;
1 cold_out;
2 before_evaporator;
3 after_evaporator;
4 separator_gas;	//if flooded evaporator: separator out
5 separator_liquid;	//if flooded evaporator: separator out
6 before_valve;		//before expansion valve, if regenerator used
7 suction;		//compressor suction, if regenerator
8 sensor_1;		//additional sensor 1
9 sensor_2;		//additional sensor 2
A crankcase;		//compressor case
B regenerator;
C afrer_condenser;
D before_condenser;
E hot_out;
F hot_in;
*/

String fw_version = "1.13";

//hardware resources
#define RELAY_HEATPUMP        	A2
#define RELAY_HOTSIDE_CIRCLE  	A1

#define PR_LOW		A6
#define PR_HIGH		A7

#define OW_BUS_ALLTSENSORS    9
#define speakerOut            6
#define em_pin1               A3


String hw_version = "v1.1+";

#define LATCH_595 		3
#define CLK_595 		2
#define DATA_595 		7
#define OE_595 			4

//---------------------------memory debug
#ifdef __arm__
	// should use uinstd.h to define sbrk but Due causes a conflict
	extern "C" char* sbrk(int incr);
#else // __ARM__
	extern char *__brkval;
#endif // __arm__
     
int freeMemory() {
	char top;
	#ifdef __arm__
		return &top - reinterpret_cast<char*>(sbrk(0));
	#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
		return &top - __brkval;
	#else // __arm__
		return __brkval ? &top - __brkval : &top - __malloc_heap_start;
	#endif // __arm__
}
//---------------------------memory debug END

    
#include <avr/wdt.h>
#include <EEPROM.h>

#define SEED 	0xFFFF
#define POLY	0xA001
unsigned int 	crc16;
int 		cf;
#define MODBUS_MR 50	//50 ok now

#include <SoftwareSerial.h>
#define SerialRX        12	//RX connected to RO - Receiver Output  
#define SerialTX        11	//TX connected to DI - Driver Output Pin
#define SerialTxControl 13	//RS485 Direction control DE and RE to this pin
#define RS485Transmit    HIGH
#define RS485Receive     LOW

SoftwareSerial RS485Serial(SerialRX, SerialTX); // RX, TX

#include <OneWire.h>
#include <DallasTemperature.h>
//library's DEVICE_DISCONNECTED_C -127.0

OneWire ow_ALLTSENSORS(OW_BUS_ALLTSENSORS);
DallasTemperature s_allTsensors(&ow_ALLTSENSORS);

DeviceAddress dev_addr;		//temp


//short names used to prevent unreadeable source
#ifdef T_cold_in
	bool TciE = 1;
#else
	bool TciE = 0;
#endif
double Tci = -127.0;

#ifdef T_cold_out
	bool TcoE = 1;
#else
	bool TcoE = 0;
#endif
double Tco = -127.0;

#ifdef T_before_evaporator
	bool TbeE = 1;
#else
	bool TbeE = 0;
#endif
double Tbe = -127.0;


#ifdef T_after_evaporator
	bool TaeE = 1;
#else
	bool TaeE = 0;
#endif
double Tae = -127.0;


/*
#ifdef T_separator_gas
	bool TsgE = 1;
#else
	bool TsgE = 0;
#endif
double Tsg = -127.0;


#ifdef T_separator_liquid
	bool TslE = 1;
#else
	bool TslE = 0;
#endif
double Tsl = -127.0;


#ifdef T_before_valve
	bool TbvE = 1;
#else
	bool TbvE = 0;
#endif
double Tbv = -127.0;


#ifdef T_suction
	bool TsucE = 1;
#else
	bool TsucE = 0;
#endif
double Tsuc = -127.0;
*/

#ifdef T_sensor_1
	bool Ts1E = 1;
#else
	bool Ts1E = 0;
#endif
double Ts1 = -127.0;


#ifdef T_sensor_2
	bool Ts2E = 1;
#else
	bool Ts2E = 0;
#endif
double Ts2 = -127.0;	


#ifdef T_crc
	bool TcrcE = 1;
#else
	bool TcrcE = 0;
#endif
double Tcrc = -127.0;

#ifdef T_regenerator
	bool TregE = 1;
#else
	bool TregE = 0;
#endif
double Treg = -127.0;


#ifdef T_afrer_condenser
	bool TacE = 1;
#else
	bool TacE = 0;
#endif
double Tac = -127.0;

#ifdef T_before_condenser
	bool TbcE = 1;
#else
	bool TbcE = 0;
#endif
double Tbc = -127.0;

#ifdef T_hot_out
	bool ThoE = 1;
#else
	bool ThoE = 0;
#endif
double Tho = -127.0;

#ifdef T_hot_in
	bool ThiE = 1;
#else
	bool ThiE = 0;
#endif
double Thi = -127.0;

double T_setpoint 			= T_SETPOINT;
double T_setpoint_lastsaved		= T_setpoint;
double T_EEV_setpoint 			= EEV_TARGET_TEMP_DIFF;
double T_EEV_dt				= 0.0;		//real, used during run
const double cT_setpoint_max 		= T_SETPOINT_MAX;
const double cT_setpoint_min 		= T_SETPOINT_MIN;
//const double cT_hotcircle_delta_min 	= T_HOTCIRCLE_DELTA_MIN;
const double cT_crc_min 		= T_CRANKCASE_MIN;
const double cT_crc_max 		= T_CRANKCASE_MAX;
const double cT_crc_heat_threshold 	= T_CRANKCASE_HEAT_THRESHOLD;
//const double cT_reg_heat_threshold 	= T_REG_HEAT_THRESHOLD;
const double cT_before_condenser_max 	= T_BEFORE_CONDENSER_MAX;
const double cT_coldref_min 		= T_COLDREF_MIN;
const double cT_before_evap_work_min 	= T_BEFORE_EVAP_WORK_MIN;
const double cT_cold_min 		= T_COLD_MIN;
const double cT_hot_max 		= T_HOT_MAX;
//const double cT_workingOK_cold_delta_min = 0.5; 	// 0.7 - 1st try, 2nd try 0.5
//const double cT_workingOK_hot_delta_min	= 0.5;
const double cT_workingOK_crc_min 	= T_WORKINGOK_CRANKCASE_MIN;        	//need to be not very high to normal start after deep freeze
const double c_wattage_max 		= MAX_WATTS;   	//FUNAI: 1000W seems to be normal working wattage INCLUDING 1(one) CR25/4 at 3rd speed
							//PH165X1CY : 920 Watts, 4.2 A
const double c_workingOK_wattage_min 	= c_wattage_max/5;     //

unsigned int pr_low_state_anal  	= 0;	//sensors are NC for spec. conditions, so 1 == ok, 0 == error
unsigned int pr_high_state_anal    	= 0;	//

bool pr_low_state_bool  	= 1;	//sensors are NC for spec. conditions, so 1 == ok, 0 == error
bool pr_high_state_bool    	= 1;	//

bool heatpump_state    		= 0;
bool hotside_circle_state  	= 0;
bool coldside_circle_state 	= 0;
bool crc_heater_state    	= 0;
//bool reg_heater_state		= 0;

//bool relay6_state		= 0;
//bool relay7_state		= 0;

bool LED_OK_state		= 0;
bool LED_ERR_state		= 0;

bool S0_state			= 0;
bool S1_state			= 0;
bool S2_state			= 0;
bool S3_state			= 0;

bool EEV1_state			= 0;
bool EEV2_state			= 0;
bool EEV3_state			= 0;
bool EEV4_state			= 0;

const long poweron_pause     	= POWERON_PAUSE    ;    	//default 5 mins
const long mincycle_poweroff 	= MINCYCLE_POWEROFF;    	//default 5 mins
const long mincycle_poweron  	= MINCYCLE_POWERON ;  		//default 60 mins
bool _1st_start_sleeped 		= 0;
//??? TODO: periodical start ?
//const long floor_circle_maxhalted = 6000000;  //circle NOT works max 100 minutes
const long deffered_stop_hotcircle = DEFFERED_STOP_HOTCIRCLE;

int EEV_cur_pos		= 0;
int EEV_reopen_pos	= 0;
bool EEV_must_reopen_flag	= 0;

int EEV_apulses		= 0;		//for async
bool EEV_adonotcare	= 0;		
const unsigned char EEV_steps[4] = {0b1010, 0b0110, 0b0101, 0b1001};
char EEV_cur_step 	= 0;
bool EEV_fast		= 0;		
#ifdef EEV_MANUAL
	bool EEV_manual		= 1;
#else
	bool EEV_manual		= 0;
#endif
const bool c_EEV_reopenlast	= EEV_REOPENLAST;

//main cycle vars
unsigned long millis_prev	= 0;
unsigned long millis_now	= 0;
unsigned long millis_cycle	= 1000;

unsigned long millis_last_heatpump_on  = 0;
unsigned long millis_last_heatpump_off = 0;

unsigned long millis_last_hotWP_on  = 0;
unsigned long millis_last_hotWP_off = 0;

unsigned long millis_last_coldWP_off = 0;

unsigned long millis_notification  		= 0;
unsigned long millis_notification_interval 	= 33000;

unsigned long millis_displ_update          	= 0;
unsigned long millis_displ_update_interval 	= 10000;

unsigned long millis_escinput_485	=  0;  
unsigned long millis_charinput_485 	=  0;  
unsigned long millis_escinput_local	=  0;  
unsigned long millis_charinput_local 	=  0;  


unsigned long millis_lasteesave	=  0;  

unsigned long millis_last_printstats = 0;

unsigned long millis_eev_last_close 	=  0;
unsigned long millis_eev_last_on  	=  0;
unsigned long millis_eev_last_step 	= 0;
unsigned long millis_eev_minworkpos_time =  0;
unsigned long millis_eev_last_work 	=  0;

unsigned long tmic1    =  0;
unsigned long tmic2    =  0;

int skipchars_485 = 0;
int skipchars_local = 0;

#define BUFSIZE          150   

unsigned char dataBuf[BUFSIZE+1];	// Allocate some space for the string, do not change that size!
char inChar= -1;      		// space to store the character read
byte index = 0;       		// Index into array; where to store the character

//-------------temporary variables
char temp[10];
int i	= 0;
int u	= 0;
int z	= 0;
int x	= 0;
int y	= 0;
double tempdouble	= 0.0;
double tempdouble_intpart	= 0.0;

int tempint       	= 0;
bool tempbool 		= 0;

char 		fp_integer	= 0;
char		fp_fraction	= 0;

String outString;
String lastStopCauseTxt;		//20 reserved, but use 12 chars of text max
bool fl_printSS_lastStopCauseTxt = 0;	//flag to call printSS
#define LSCint_normal		0
#define LSCint_protective	1
#define LSCint_error		2
int 	LSCint	=	LSCint_normal;	//0 = normal, 1 = protective, 2 = error
String lastStartMsgTxt;			//same as LSC
bool fl_printSS_lastStartMsgTxt = 0;	//flag to call printSS
String t_sensorErrString;

char convBuf[13];

//-------------EEPROM
int eeprom_magic_read = 0x00;
int eeprom_addr       = 0x00;      
//initial values, saved to EEPROM and can be modified later
//CHANGE eeprom_magic after correction!
const int eeprom_magic = MAGIC;

//-------------ERROR states
#define ERR_OK       	0
#define ERR_T_SENSOR   	1
#define ERR_P_HI	2
#define ERR_P_LO	3

int errorcode 			= 0;
unsigned char sequential_errors = 0;

//--------------------------- for wattage 
#define ADC_BITS 10                      //10 fo regular arduino
#define ADC_COUNTS (1<<ADC_BITS)
float em_calibration  	= 62.5;
int em_samplesnum   	= 2960;   	// Calculate Irms only 1480 == full 14 periods for 50Hz, 2960 = 28, 4440 = 42
//double Irms       	= 0;      	//for tests with original procedure
int supply_voltage   	= 0;
int em_i            	= 0;
//phase 1
int sampleI_1       	= 0;
double filteredI_1  	= 0;
double offsetI_1    	= ADC_COUNTS>>1;	//Low-pass filter output
double sqI_1,sumI_1 	= 0;            	//sq = squared, sum = Sum, inst = instantaneous
double async_Irms_1 	= 0;
double async_wattage 	= 0;
//--------------------------- for wattage END

const char str1[] PROGMEM = "Valden Heat Pump Controller, https://github.com/OpenHP/\n\r\n\rCommands: \n\r(?) help\n\r(-) decrease setpoint T\n\r\n\r(+) increase setpoint T";
const char str2[] PROGMEM = "(<) decrease EEV T diff \n\r(>) increase EEV T diff\n\r\n\r(M) manual EEV mode\n\r(A) auto EEV mode\n\r\n\r(z) -1 EEV\t(Z) -10 EEV\n\r(x) +1 EEV\t(X) +10 EEV\n\r(G) get stats";
const char str3[] PROGMEM = "EEV:auto";
const char str4[] PROGMEM = "EEV:manual";
const char str5[] PROGMEM = "N/A,auto";
const char str6[] PROGMEM = "+10 ok";
const char str7[] PROGMEM = "-10 ok";
const char str8[] PROGMEM = "+1 ok";
const char str9[] PROGMEM = "-1 ok";
const char str10[] PROGMEM = "Max!";
const char str11[] PROGMEM = "Min!";
const char str12[] PROGMEM = "HWP ON by Setp. update";
const char str13[] PROGMEM = "EE->mem";
const char str14[] PROGMEM = "mem->EE";
const char str15[] PROGMEM = "OK:E.T.Sens.";
const char str16[] PROGMEM = "OK:Pr.Cold";
const char str17[] PROGMEM = "OK:Pr.Hot";
const char str18[] PROGMEM = "HWP_ON";
const char str19[] PROGMEM = "unkn_F";

PGM_P const const_strs[] PROGMEM = {
	str1,	str2,	str3,	str4,	str5,	str6,	str7,	str8,	str9,	str10,
	str11,	str12,	str13,	str14,	str15,	str16,	str17,	str18,	str19
};

#define IDX_HELP1 	0
#define IDX_HELP2 	1
#define IDX_EEVAUTO 	2
#define IDX_EEVMANUAL	3
#define IDX_NAAUTO	4
#define IDX_PLUS10_OK	5
#define IDX_MINUS10_OK	6
#define IDX_PLUS1_OK	7
#define IDX_MINUS1_OK	8
#define IDX_MAX		9
#define IDX_MIN		10
#define IDX_HWP_ONBYUPD	11
#define IDX_EEtoMEM	12
#define IDX_MEMtoEE	13
#define IDX_OK_ETSENS	14
#define IDX_OK_PRCOLD	15
#define IDX_OK_PRHOT	16
#define IDX_HWPON	17
#define IDX_UNKNF	18


//--------------------------- functions
long ReadVcc() {
	// Read 1.1V reference against AVcc
	// set the reference to Vcc and the measurement to the internal 1.1V reference
	#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
		ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
		ADMUX = _BV(MUX5) | _BV(MUX0);
	#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
		ADMUX = _BV(MUX3) | _BV(MUX2);
	#else
		ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	#endif  

	delay(2); // Wait for Vref to settle
	ADCSRA |= _BV(ADSC); // Start conversion
	while (bit_is_set(ADCSRA,ADSC)); // measuring
	
	uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
	uint8_t high = ADCH; // unlocks both
	
	long result = (high<<8) | low;
	//constant NOT same as in battery controller!
	result = 1126400L / result; // Calculate Vcc (in mV); (me: !!) 1125300  (!!) = 1.1*1023*1000
	return result; // Vcc in millivolts
}

/*void PrintS (String str) {
	#ifdef RS485_HUMAN
		char *outChar=&str[0];  
		digitalWrite(SerialTxControl, RS485Transmit);
		halifise();
		delay(1);    
		RS485Serial.print(outChar);
		RS485Serial.println();
		RS485Serial.flush();
		digitalWrite(SerialTxControl, RS485Receive);  
	#endif
}*/

void PrintSS (String str) {
	char *outChar=&str[0];  
	if (str == "") {
		return;
	}
	#ifdef RS485_HUMAN
		digitalWrite(SerialTxControl, RS485Transmit);
		halifise();
		delay(1);    
		RS485Serial.print(outChar);
		RS485Serial.println();
		RS485Serial.flush();
		digitalWrite(SerialTxControl, RS485Receive);  
	#endif
	Serial.println(outChar);
	Serial.flush();
}

void PrintSSch(char idx) {
	strcpy_P(dataBuf, (PGM_P)pgm_read_word(&const_strs[idx]));
	Serial.println((const char *) dataBuf);
	#ifdef RS485_HUMAN
		digitalWrite(SerialTxControl, RS485Transmit);
		halifise();
		delay(1);    
		RS485Serial.print((const char *) dataBuf);
		RS485Serial.println();
		RS485Serial.flush();
		digitalWrite(SerialTxControl, RS485Receive);  
	#endif
}
void PrintSS_SaD (double num) {	//global string + double
	#ifdef RS485_HUMAN
		digitalWrite(SerialTxControl, RS485Transmit);
		halifise();
		delay(1);
		RS485Serial.print(outString);
		RS485Serial.println(num);
		RS485Serial.flush();
		digitalWrite(SerialTxControl, RS485Receive);  
	#endif
	Serial.print(outString);
	Serial.println(num);
	Serial.flush();
}

void PrintSS_SaBl (bool num) {	
	#ifdef RS485_HUMAN
		digitalWrite(SerialTxControl, RS485Transmit);
		halifise();
		delay(1);
		RS485Serial.print(outString);
		RS485Serial.println(num);
		RS485Serial.flush();
		digitalWrite(SerialTxControl, RS485Receive);  
	#endif
	Serial.print(outString);
	Serial.println(num);
	Serial.flush();
}

void ApToOut_D (double num) {
	outString += String(num);
}

void PrintSS_SaI (int num) {	
	#ifdef RS485_HUMAN
		digitalWrite(SerialTxControl, RS485Transmit);
		halifise();
		delay(1);
		RS485Serial.print(outString);
		RS485Serial.println(num);
		RS485Serial.flush();
		digitalWrite(SerialTxControl, RS485Receive);  
	#endif
	Serial.print(outString);
	Serial.println(num);
	Serial.flush();
}


/*void PrintSS_SaI (int num) {	//global string + double
	#ifdef RS485_HUMAN
		digitalWrite(SerialTxControl, RS485Transmit);
		halifise();
		delay(1);
		RS485Serial.print(outString);
		RS485Serial.println(num);
		RS485Serial.flush();
		digitalWrite(SerialTxControl, RS485Receive);  
	#endif
	Serial.print(outString);
	Serial.println(num);
	Serial.flush();
}*/

void _PrintHelp(void) {   
	PrintSS("fw: "	+ fw_version  + " board: "+ hw_version);
	PrintSSch(IDX_HELP1);
	#ifndef NO_EEV
		PrintSSch(IDX_HELP2);
	#endif
}

void PrintSS_double (double double_to_print) {
	dtostrf(double_to_print,1,2,temp);
	PrintSS(temp);
}

void Add_Double_To_Buf_IntFract (double float_to_convert) {	//uses tempdouble tempdouble_intpart fp_integer fp_fraction
	if (float_to_convert > 255.0 || float_to_convert < -127.0) {
		fp_integer	= -127;
		fp_fraction	= 0;
	} else {
		tempdouble = modf (float_to_convert , &tempdouble_intpart);
		fp_integer = trunc(tempdouble_intpart);
		tempdouble  = tempdouble * 100;
		fp_fraction = round(tempdouble);
	}
	dataBuf[i]  =  fp_integer;
	i++;
	dataBuf[i]  =  fp_fraction;
	i++;
	/*
	Serial.println(float_to_convert);
	Serial.println(fp_integer, DEC);
	Serial.println(fp_fraction, DEC);*/
}


void IntFract_to_tempdouble (char _int_to_convert, char _fract_to_convert) {	//fract is also signed now!
	tempdouble = (double) _fract_to_convert / 100;
	tempdouble += _int_to_convert;
	/*Serial.println(_int_to_convert);
	Serial.println(_fract_to_convert);
	Serial.println(tempdouble);*/
}


void _ProcessInChar(void){
	//remote commands +,-,G,0x20/?/Enter/A/M/x/X/z/Z
	switch (inChar) {
		case 0x00:
			break;
		case 0x20:
			_PrintHelp();
			break;
		case 0x3F:
			_PrintHelp();
			break;
		case 0x0D:
			_PrintHelp();
			break;
		case 0x2B:
			Inc_T();
			break;
		case 0x2D:
			Dec_T();
			break;
		#ifndef NO_EEV
			case 0x3C:
				Dec_E();
				break;
			case 0x3E:
				Inc_E();
				break;
			case 0x41:
				EEV_manual = 0;
				PrintSSch(IDX_EEVAUTO);
				break;
		#endif
		case 0x47:
			PrintStats_SS();
			millis_last_printstats = millis_now;
			break;
		#ifndef NO_EEV
			case 0x4D:
				EEV_manual = 1;
				PrintSSch(IDX_EEVMANUAL);
				break;
			case 0x58:	//+10
				if (EEV_manual != 1){
					PrintSSch(IDX_NAAUTO);
					break;
				}
				EEV_apulses += 10;
				EEV_fast = 1;
				PrintSSch(IDX_PLUS10_OK);
				break;
			case 0x5A:	//-10
				if (EEV_manual != 1){
					PrintSSch(IDX_NAAUTO);
					break;
				}
				EEV_apulses -= 10;
				EEV_fast = 1;
				PrintSSch(IDX_MINUS10_OK);
				break;
			case 0x78:	//+1
				if (EEV_manual != 1){
					PrintSSch(IDX_NAAUTO);
					break;
				}
				EEV_apulses += 1;
				EEV_fast = 1;
				PrintSSch(IDX_PLUS1_OK);
				break;
			case 0x7A:	//-1
				if (EEV_manual != 1){
					PrintSSch(IDX_NAAUTO);
					break;
				}
				EEV_apulses += 10;
				EEV_fast = 1;
				PrintSSch(IDX_MINUS1_OK);
				break;
		#endif
	}

}

int Inc_T (void) {
	if (T_setpoint + 0.5 > cT_setpoint_max) {
		PrintSSch(IDX_MAX);
		delay (200);
		return 0;
	}
	T_setpoint += 0.5;
	PrintSS_double(T_setpoint);	
	return 1;	
}

int Dec_T (void) {
	if (T_setpoint - 0.5 < cT_setpoint_min) {
		PrintSSch(IDX_MIN);
		delay (200);
		return 0;
	}
	T_setpoint -= 0.5;
	PrintSS_double(T_setpoint);	
	return 1;
}

int Inc_E (void) {		///!!! unprotected
	T_EEV_setpoint += 0.25;
	PrintSS_double(T_EEV_setpoint);
	return 1;	
}

int Dec_E (void) {		///!!! unprotected
	T_EEV_setpoint -= 0.25;
	PrintSS_double(T_EEV_setpoint);
	return 1;
}





void _HotWPon_by_Setpoint_update(void){	//if setpoint updated: start hot circle to check temperature
	if (  (heatpump_state == 0)   &&  (hotside_circle_state  == 0)  && ((unsigned long)(millis_now - millis_last_hotWP_on) < HOTCIRCLE_START_EVERY)	) {    //process START_EVERY for hot side
		millis_last_hotWP_off = millis_now;
		hotside_circle_state  = 1;
		PrintSSch(IDX_HWP_ONBYUPD);
	}
}

void PrintStats_SS (void) {
		
		if (TciE)	{ 	outString = F("\n\r---\n\r\tTbe:\t")	;	PrintSS_SaD(Tbe);	}
	        if (TaeE)	{	outString = F("\tTae:\t")	;	PrintSS_SaD(Tae);	}
		if (TcoE)	{	outString = F("\tTci:\t");		PrintSS_SaD(Tci);	}
		if (TcoE)	{	outString = F("\tTco:\t")	;	PrintSS_SaD(Tco);	}
	        
	        //if (TsgE)	{	outString = F("\tTsg: ")	;	PrintSS_SaD(Tsg);	}
	        //if (TslE)	{	outString = F("\tTsl: ")	;	PrintSS_SaD(Tsl);	}
	        //if (TbvE)	{	outString = F("\tTbv: ")	;	PrintSS_SaD(Tbv);	}
	        //if (TsucE)	{	outString = F("\tTsuc: ")	;	PrintSS_SaD(Tsuc);	}
	        if (Ts1E)	{	outString = F("\tTs1:\t")	;	PrintSS_SaD(Ts1);	}
	        if (Ts2E)	{	outString = F("\tTs2:\t")	;	PrintSS_SaD(Ts2);	}
	        //Tcrc misorder due to large string
	        if (TregE)	{	outString = F("\tTreg:\t")	;	PrintSS_SaD(Treg);	}
	        if (TbcE)	{	outString = F("\tTbc:\t")	;	PrintSS_SaD(Tbc);	}
		if (TacE)	{	outString = F("\tTac:\t")	;	PrintSS_SaD(Tac);	}
	        if (ThiE)	{	outString = F("\tThi:\t")	;	PrintSS_SaD(Thi);	}
		if (ThoE)	{	outString = F("\tTho:\t")	;	PrintSS_SaD(Tho);	}
		if (TcrcE)	{	outString = F("\tTcrankcase:\t");	PrintSS_SaD(Tcrc);	}//misorder due to large string
		outString = F("\tSetpoint:\t");	
		PrintSS_SaD(T_setpoint); 
		
		outString = F("\n\r\tHP:\t");
		PrintSS_SaBl(heatpump_state);
		outString = F("\tHWP:\t");
		PrintSS_SaBl(hotside_circle_state);
		outString = F("\tCWP:\t");
		PrintSS_SaBl(coldside_circle_state);
		outString = F("\tCRCheat:");
		PrintSS_SaBl(crc_heater_state);
		outString = F("\tWatts:\t")	;	
		PrintSS_SaD(async_wattage);
		
		#ifndef NO_EEV
			outString = F("\n\r\tT_EEV_setpoint: ");
			PrintSS_SaD(T_EEV_setpoint);
			outString = "\tEEV_pos:\t";
			PrintSS_SaI(EEV_cur_pos);
		#endif 
		
		outString = "\n\r\tErr:\t";
		PrintSS_SaI(errorcode);
		outString = F("\tPr.Cold:")	;	
		if (pr_low_state_bool == 1) {
			outString += F("OK");
		} else {
			outString += F("ERR");
		}
		outString += F("\n\r\tPr.Hot:\t")	;	
		if (pr_high_state_bool == 1) {
			outString += F("OK");
		} else {
			outString += F("ERR");
		}
		
		outString += F("\n\r\n\r\tLast Stop Cause:\t");
		outString += lastStopCauseTxt;
		outString += F("\n\r\tLast Start Message:\t");
		outString += lastStartMsgTxt;
		outString += F("\n\r---\n\r");
		PrintSS(outString);
		
	#ifdef RS485_HUMAN
		digitalWrite(SerialTxControl, RS485Transmit);
		halifise();
		delay(1);    
		RS485Serial.print(outString);
		RS485Serial.println();
		RS485Serial.flush();
		digitalWrite(SerialTxControl, RS485Receive);  
	#endif
}

void Calc_CRC(unsigned char b) {	//uses/changes y
	crc16 ^= b & 0xFF;
	for (y=0; y<8; y++) {
		cf = crc16 & 0x0001;
		crc16>>=1;
		if (cf) { crc16 ^= POLY; }
	}
}

void CheckIsInvalidCRCAddr(unsigned char *addr) {
	if (OneWire::crc8( addr, 7) != addr[7] ) {
		i+= 1;
	}
}

void WriteFloatEEPROM(int addr, float val) { 
	byte *x = (byte *)&val;
	for(byte u = 0; u < 4; u++) EEPROM.write(u+addr, x[u]);
}
 
float ReadFloatEEPROM(int addr) {   
	byte x[4];
	for(byte u = 0; u < 4; u++) x[u] = EEPROM.read(u+addr);
	float *y = (float *)&x;
	return y[0];
}

void SaveSetpointEE(void) {
	if(  	(T_setpoint_lastsaved != T_setpoint) && 
		( ((unsigned long)(millis_now - millis_lasteesave) > 15*60*1000 )  ||  (millis_lasteesave == 0) )  ) {
			eeprom_addr = 1;
			WriteFloatEEPROM(eeprom_addr, T_setpoint);
			millis_lasteesave = millis_now;
			T_setpoint_lastsaved = T_setpoint;
		}
}

double GetT (int channel) {
	S0_state = bitRead(channel,0);
	S1_state = bitRead(channel,1);
	S2_state = bitRead(channel,2);
	S3_state = bitRead(channel,3);
	halifise();
	
	tempdouble = -127.0;
	for ( i = 0; i < 8; i++) {
		#ifdef WATCHDOG
			wdt_reset();
		#endif
		eevise();
		tempdouble = s_allTsensors.getTempCByIndex(0);
		if ( (tempdouble == 85.0) || (tempdouble < -55.0) || (tempdouble == 0.0) || (tempdouble > 125.0) ) {	//0.0 added to test
			//outString =  F("Warn:T_SensReRead!");
			//PrintSS_SaD(tempdouble);
			if ( tempdouble == 85.0 || tempdouble == 0.0 ) {    //initial value in dallas register after poweron
				s_allTsensors.requestTemperatures();	//!!!added to test, seems to work ok
				delay (375);              //375 actual for 11 bits resolution, 2-3 retries OK for 12-bits resolution
			} else {
				delay (37);
			}
		} else {
			break;
		}
	}
	s_allTsensors.requestTemperatures();
	if ( (tempdouble > 125.0) || (tempdouble < -55.0)) {	//incorrect readings protection, rare
		tempdouble = -127.0;
	}
	return tempdouble;
}

//older version of GetT
/*double GetT (int channel) {
	S0_state = bitRead(channel,0);
	S1_state = bitRead(channel,1);
	S2_state = bitRead(channel,2);
	S3_state = bitRead(channel,3);
	halifise();
	
	tempdouble = -127.0;
	for ( i = 0; i < 8; i++) {
		#ifdef WATCHDOG
			wdt_reset();
		#endif
		eevise();
		tempdouble = s_allTsensors.getTempCByIndex(0);
		if ( (tempdouble == 85.0) || (tempdouble == -127.0) ) {
			if ( tempdouble == 85.0 ) {    //initial value in dallas register after poweron
				s_allTsensors.requestTemperatures();//!!! added to test
				delay (375);              //375 actual for 11 bits resolution, 2-3 retries OK for 12-bits resolution
			} else {
				delay (37);
			}
		} else {
			break;
		}
	}
	s_allTsensors.requestTemperatures();
	return tempdouble;
}*/

void GetTemperatures(void){
	if (TciE)	{ 	Tci	= GetT(0);}
	if (TcoE)	{	Tco	= GetT(1);}
	if (TbeE)	{	Tbe	= GetT(2);}
	if (TaeE)	{	Tae	= GetT(3);}
	//if (TsgE)	{	Tsg	= GetT(4);}
	//if (TslE)	{	Tsl	= GetT(5);}
	//if (TbvE)	{	Tbv	= GetT(6);}
	//if (TsucE)	{	Tsuc	= GetT(7);}
	if (Ts1E)	{	Ts1	= GetT(8);}
	if (Ts2E)	{	Ts2	= GetT(9);}
	if (TcrcE)	{	Tcrc	= GetT(10);}
	if (TregE)	{	Treg	= GetT(11);}
	if (TacE)	{	Tac	= GetT(12);}
	if (TbcE)	{	Tbc	= GetT(13);}
	if (ThoE)	{	Tho	= GetT(14);}
	if (ThiE)	{	Thi	= GetT(15);}	
}

void on_EEV(){	
	x = EEV_steps[EEV_cur_step];
	EEV1_state	=	bitRead(x, 0);
	EEV2_state	=	bitRead(x, 1);
	EEV3_state	=	bitRead(x, 2);
	EEV4_state	=	bitRead(x, 3);
	halifise();
}

void off_EEV(){	
	EEV1_state	=	0;
	EEV2_state	=	0;
	EEV3_state	=	0;
	EEV4_state	=	0;
	//PrintSS("off_EEV");
	halifise();
}

void halifise(void){
	/*
	relay 1: heat pump
	relay 2: hot side circulator pump
	relay 3: cold side circulator pump
	relay 4: crankcase heater
	(no more v1.3mi) relay 5:
	
	#define RELAY_HEATPUMP        	A2
	#define RELAY_HOTSIDE_CIRCLE  	A1
	
	Reg 1:
	595.0: 4067 S3
	595.1: 4067 S0
	595.2: 4067 S1
	595.3: 4067 S2
	595.4: EEV_1
	595.5: EEV_2
	595.6: EEV_3
	595.7: EEV_4
	
	Reg 2:
	595.8: !! free
	595.9: ok/err LED 2
	595.A: Relay 6 
	595.B: Relay 7 
	595.C: Relay 5 
	595.D: Relay 4
	595.E: Relay 3
	595.F: ok/err LED 1
	
	Reg 3:
	595.10: LED "EEV opening"
	595.11: LED "EEV closing"
	595.12: LED "EEV Fast"
	595.13: LED "485 RX"
	595.14: LED "485 TX"
	595.15: LED "Manual mode"
	595.16: LED "LSC: error"
	595.17: LED "LSC: protection"
	*/

	digitalWrite(LATCH_595, 0);
	//17
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	if (LSCint == LSCint_protective) {
		digitalWrite(DATA_595, 1);
	} else {
		digitalWrite(DATA_595, 0);
	}
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//16
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	if (LSCint == LSCint_error) {
		digitalWrite(DATA_595, 1);
	} else {
		digitalWrite(DATA_595, 0);
	}
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//15
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, EEV_manual);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//14
	tempbool = digitalRead (13);
	
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, tempbool);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//13
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, !tempbool);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//12
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, EEV_fast);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//11
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	if ( EEV_apulses < 0 ) {
		digitalWrite(DATA_595, 1);
	} else {
		digitalWrite(DATA_595, 0);
	}
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//10
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	if ( EEV_apulses > 0 ) {
		digitalWrite(DATA_595, 1);
	} else {
		digitalWrite(DATA_595, 0);
	}
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//F
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, LED_ERR_state);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//E
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, coldside_circle_state);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//D
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, crc_heater_state);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//C
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	//digitalWrite(DATA_595, reg_heater_state);
	digitalWrite(DATA_595, 0);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//B
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	//digitalWrite(DATA_595, relay7_state);
	digitalWrite(DATA_595, 0);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//A
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	//digitalWrite(DATA_595, relay6_state);
	digitalWrite(DATA_595, 0);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//9
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, LED_OK_state);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//8
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, 0);	//FREE
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//7
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, EEV4_state);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//6
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, EEV3_state);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//5
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, EEV2_state);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//4
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, EEV1_state);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//3
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, S2_state);	
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//2
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, S1_state); 
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//1
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, S0_state);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	//0
	digitalWrite(CLK_595, 0);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(DATA_595, S3_state);
	digitalWrite(CLK_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(CLK_595, 0);
	//
	digitalWrite(LATCH_595, 1);
	__asm__ __volatile__ ("nop\n\t");
	digitalWrite(LATCH_595, 0);
	digitalWrite	(RELAY_HEATPUMP,	heatpump_state);
	digitalWrite	(RELAY_HOTSIDE_CIRCLE, 	hotside_circle_state);
}

void eevise(void) {
	if (  		((( EEV_apulses < 0 ) && (EEV_fast == 1))						&&	 ((unsigned long)(millis_now - millis_eev_last_step) > (EEV_PULSE_FCLOSE_MILLIS))  	)	||
			((( EEV_apulses < 0 ) && (EEV_fast == 0))						&&	 ((unsigned long)(millis_now - millis_eev_last_step) > (EEV_PULSE_CLOSE_MILLIS)	) 	)	||
			((( EEV_apulses > 0 ) && 			(EEV_cur_pos <  EEV_MINWORKPOS  ))	&&	 ((unsigned long)(millis_now - millis_eev_last_step) > (EEV_PULSE_WOPEN_MILLIS)	) 	)	||
			((( EEV_apulses > 0 ) && (EEV_fast == 1) &&  	(EEV_cur_pos >= EEV_MINWORKPOS	)) 	&&	 ((unsigned long)(millis_now - millis_eev_last_step) > (EEV_PULSE_FOPEN_MILLIS) )	)	||	
			((( EEV_apulses > 0 ) && (EEV_fast == 0) &&  	(EEV_cur_pos >= EEV_MINWORKPOS	)) 	&&	 ((unsigned long)(millis_now - millis_eev_last_step) > (EEV_PULSE_OPEN_MILLIS)  )	)	||
			(millis_eev_last_step == 0)
			) {
			if ( EEV_apulses != 0 ) {
				if ( EEV_apulses > 0 ) {
					if (EEV_cur_pos + 1 <= EEV_MAXPULSES) {
						EEV_cur_pos += 1;
						EEV_cur_step += 1;
						EEV_apulses -= 1;
					} else {
						EEV_apulses = 0;
						//PrintSS("EEmax!");
					}
				}
				if ( EEV_apulses < 0 ) {
					if (	(EEV_cur_pos - 1 >= EEV_MINWORKPOS)	|| (EEV_adonotcare == 1) ) {
						EEV_cur_pos -= 1;
						EEV_cur_step -= 1;
						EEV_apulses += 1;
					} else {
						EEV_apulses = 0;
						//PrintSS("EEmin!");
					}
				}
				if (EEV_cur_step  > 3) EEV_cur_step = 0;
				if (EEV_cur_step  < 0) EEV_cur_step = 3;
				x = EEV_steps[EEV_cur_step];
				EEV1_state	=	bitRead(x, 0);
				EEV2_state	=	bitRead(x, 2);		//!!!here pins are swapped fot sanhua
				EEV3_state	=	bitRead(x, 1);		//!!!here pins are swapped fot sanhua
				EEV4_state	=	bitRead(x, 3);
			}
			if (EEV_cur_pos < 0) { 
				EEV_cur_pos = 0;	
			}
			millis_eev_last_step = millis_now;
			#ifdef EEV_DEBUG 
				PrintSS(String(EEV_cur_pos));	
			#endif
			halifise();
	}
}

//--------------------------- functions END

void setup(void) {
	pinMode		(LATCH_595, 		OUTPUT);
	pinMode		(CLK_595, 		OUTPUT);
	pinMode		(DATA_595, 		OUTPUT);
	pinMode		(OE_595, 		OUTPUT);
	pinMode		(RELAY_HEATPUMP,	OUTPUT);
	pinMode		(RELAY_HOTSIDE_CIRCLE, 	OUTPUT);
	pinMode		(PR_LOW, 		INPUT);
	pinMode		(PR_HIGH, 		INPUT);
	
	
	digitalWrite	(LATCH_595, 		LOW);
	digitalWrite	(CLK_595, 		LOW);
	digitalWrite	(DATA_595, 		LOW);
	digitalWrite	(OE_595, 		LOW);
	digitalWrite	(RELAY_HEATPUMP,	LOW);
	digitalWrite	(RELAY_HOTSIDE_CIRCLE, 	LOW);
	halifise();
	
	#ifdef WATCHDOG
		wdt_disable();
		delay(2000);
	#endif
	
	// start serial port
	Serial.begin(9600);
	//Serial.print("Starting, dev_id:");
	//Serial.println(devID);

	RS485Serial.begin(9600);
	pinMode(SerialTxControl, OUTPUT); 
	pinMode(SerialTX, OUTPUT);   	
	pinMode(SerialRX, INPUT);   
	digitalWrite(SerialTxControl, RS485Receive);
	delay(100);
	PrintSS("ID: 0x" + String(devID, HEX));

	delay(200);
	off_EEV();
	
	pinMode	(em_pin1, INPUT);
	
	//PrintSS("setpoint (C):");
	//PrintSS(setpoint);
	
	//PrintSS(String(freeMemory()));
	
	s_allTsensors.begin();
	s_allTsensors.setWaitForConversion(false);  //ASYNC mode, request before get, see Dallas library for details
	
	//----------------------------- self-tests ----------------------------- ----------------------------- ----------------------------- 
	/*
	index = 0;
	outChar[index] = 0xFF;
	index++;
	outChar[index] = 0xAA;
	index++;
	outChar[index] = 0xBB;
	index++;
	outChar[index] = 0xCC;
	index++;
	
	crc16 = SEED;
	for (z = 0; z < index; z++) {   
		Calc_CRC(outChar[z]); 
        }
        outChar[index]=crc16 & 0xFF;
        index++;
	outChar[index]=crc16 >> 8;
	index++;
	outChar[index]=0x00;
	index++;
	
	Serial.println(crc16, HEX);
	for (z = 0; z < index; z++) {   
		Serial.print(" ");
		Serial.print(outChar[z], HEX); 
        }
	Serial.println(" ");
	*/
	
	//Relays self-test
	#if (defined SELFTEST_RELAYS_LEDS_SPEAKER || defined SELFTEST_EEV || defined SELFTEST_T_SENSORS)
		while ( 1 == 1) {
			#if defined SELFTEST_RELAYS_LEDS_SPEAKER 
				PrintSS(F("Relays and LEDS self-test"));
				
				analogWrite(speakerOut, 10);
				delay (1500);
				analogWrite(speakerOut, 0);
			
				heatpump_state    	= 1;	halifise();	delay(1000);
				hotside_circle_state  	= 1;	halifise();	delay(1000);
				coldside_circle_state 	= 1;	halifise();	delay(1000);
				crc_heater_state    	= 1;	halifise();	delay(1000);
				//reg_heater_state 	= 1;	halifise();	delay(1000);
				
				//relay6_state		= 1;	halifise();	delay(1000);
				//relay7_state		= 1;	halifise();	delay(1000);
			
				EEV_apulses		= 10;	halifise();	delay(1000);
				EEV_apulses		= -10;	halifise();	delay(1000);
				EEV_fast		= 1;	halifise();	delay(1000);
				digitalWrite(SerialTxControl, RS485Transmit);	halifise();	delay(1000);
				EEV_manual		= 1;		halifise();	delay(1000);
				LSCint 		= LSCint_error;		halifise();	delay(1000);
				LSCint 		= LSCint_protective;	halifise();	delay(1000);	
				
				LED_OK_state		= 1;	halifise();	delay(1000);
				LED_ERR_state		= 1;	halifise();	delay(1000);
			
				analogWrite(speakerOut, 10);
				delay (1500);
				analogWrite(speakerOut, 0);
				
				heatpump_state    	= 0;	halifise();	delay(1000);
				hotside_circle_state  	= 0;	halifise();	delay(1000);
				coldside_circle_state 	= 0;	halifise();	delay(1000);
				crc_heater_state    	= 0;	halifise();	delay(1000);
				//reg_heater_state 	= 0;	halifise();	delay(1000);
				
				//relay6_state		= 0;	halifise();	delay(1000);
				//relay7_state		= 0;	halifise();	delay(1000);
				
				EEV_apulses		= 10;	halifise();	delay(1000);
				EEV_apulses		= -10;	halifise();	delay(1000);
				EEV_fast		= 0;	halifise();	delay(1000);
				digitalWrite(SerialTxControl, RS485Receive);	halifise();	delay(1000);
				digitalWrite(SerialTxControl, RS485Transmit);	halifise();	delay(1000);
				EEV_manual		= 0;		halifise();	delay(1000);		
				LSCint 		= LSCint_error;		halifise();	delay(1000);
				LSCint 		= LSCint_protective;	halifise();	delay(1000);
				
				LED_OK_state		= 0;	halifise();	delay(1000);
				LED_ERR_state		= 0;	halifise();	delay(1000);
		
				analogWrite(speakerOut, 10);
				delay (1500);
				analogWrite(speakerOut, 0);
			#endif
			#if defined SELFTEST_EEV
				EEV_apulses		= 0;
				EEV_fast		= 0;
				halifise();	
				delay(1000);
				//EEV self-test, also can be used for compressor test
				//vacuuming/charge via low pressure side: leave EEV opened
				//PrintSS("EEV self-test");
				EEV_apulses =  -(EEV_MAXPULSES + EEV_CLOSE_ADD_PULSES);
				EEV_adonotcare = 1;
				EEV_fast = 1;
				while (EEV_apulses < 0){
					millis_now = millis();
					eevise();
				}
				analogWrite(speakerOut, 10);
				delay (1500);
				analogWrite(speakerOut, 0);
				delay(1000);
				//EEV_apulses =  EEV_MAXPULSES;
				EEV_apulses =  50;
				EEV_fast = 1;
				while (EEV_apulses > 0){
					millis_now = millis();
					eevise();
				}
				analogWrite(speakerOut, 10);
				delay (1500);
				analogWrite(speakerOut, 0);
				delay(1000);
			#endif
			#if defined SELFTEST_T_SENSORS
				GetTemperatures();
				
				outString=F("Tbe: ");		PrintSS_SaD(Tbe);
				outString=F("Tae: ");		PrintSS_SaD(Tae);
				outString=F("Tci: ");		PrintSS_SaD(Tci);
				outString=F("Tco: ");		PrintSS_SaD(Tco);
				outString=F("Tbc: ");		PrintSS_SaD(Tbc);
				outString=F("Tac: ");		PrintSS_SaD(Tac);
				outString=F("Thi: ");		PrintSS_SaD(Thi);
				outString=F("Tho: ");		PrintSS_SaD(Tho);
				outString=F("Ts1: ");		PrintSS_SaD(Ts1);
				outString=F("Tcrc: ");		PrintSS_SaD(Tcrc);
				outString=F("Ts2: ");		PrintSS_SaD(Ts2);
				outString=F("Treg: ");		PrintSS_SaD(Treg);
				analogWrite(speakerOut, 10);
				delay (1500);
				analogWrite(speakerOut, 0);
				delay(1000);
			#endif

	//---------DEBUG END--------  

		}
	#endif
	
	//----------------------------- self-test END----------------------------- ----------------------------- ----------------------------- 
	
	
	eeprom_magic_read = EEPROM.read(eeprom_addr);
	eeprom_addr += 1;
	//EEPROM content: 0x00 - magic,   0x01..0x04 target value
	if (eeprom_magic_read == eeprom_magic){
		PrintSSch(IDX_EEtoMEM);
	} else {
		PrintSSch(IDX_MEMtoEE);
		WriteFloatEEPROM(eeprom_addr, T_setpoint);
		EEPROM.write(0x00, eeprom_magic);
	}
	T_setpoint = ReadFloatEEPROM(eeprom_addr);
	PrintSS_double(T_setpoint);
	//eeprom_addr += 4;
	
	T_setpoint_lastsaved = T_setpoint;

	#ifdef WATCHDOG
		wdt_enable (WDTO_8S);
	#endif
	
	GetTemperatures();
	
	outString.reserve(80);
	lastStopCauseTxt.reserve(20);
	lastStartMsgTxt.reserve(20);
	t_sensorErrString.reserve(12);
	//PrintSS(String(freeMemory()));
	
	LED_OK_state		= 1;
	
	_PrintHelp();
	
	analogWrite(speakerOut, 10);
	delay (1500); 
	analogWrite(speakerOut, 0);
	lastStopCauseTxt = F("Start Pause");
	lastStartMsgTxt = "";
}

 
void loop(void) {  
	
	digitalWrite(SerialTxControl, RS485Receive);
	millis_now = millis();
	halifise();
	eevise();
	
	if (((unsigned long)(millis_now - millis_last_printstats) > HUMAN_AUTOINFO)   ||   (millis_last_printstats == 0)  ) {
		PrintStats_SS();
		millis_last_printstats = millis_now;
	}
	//--------------------async fuctions start
	if (em_i == 0) {  
		supply_voltage = ReadVcc();
	}
	if (em_i < em_samplesnum) {
		sampleI_1 = analogRead(em_pin1);
		// Digital low pass filter extracts the 2.5 V or 1.65 V dc offset, then subtract this - signal is now centered on 0 counts.
		offsetI_1 = (offsetI_1 + (sampleI_1-offsetI_1)/1024);
		filteredI_1 = sampleI_1 - offsetI_1;
	
		// Root-mean-square method current
		// 1) square current values
		sqI_1 = filteredI_1 * filteredI_1;
		// 2) sum
		sumI_1 += sqI_1;
		
		em_i += 1;
	} else {
		em_i = 0;
		double I_RATIO = em_calibration *((supply_voltage/1000.0) / (ADC_COUNTS));
		async_Irms_1 = I_RATIO * sqrt(sumI_1 / em_samplesnum);
		async_wattage = async_Irms_1*220.0;
	
		//Reset accumulators
		sumI_1 = 0;
		
		//----------------------------- self-test !!!
		/*
		PrintSS("Async impl. results 1:  ");
		PrintSS(String(async_wattage));	           // Apparent power
		PrintSS(String(async_Irms_1));	           // Irms
		PrintSS(" voltage: ");
		PrintSS(String(supply_voltage));
		*/
		//----------------------------- self-test END
		
	}
	eevise();

	//--------------------async fuctions END    
	
	if ( heatpump_state == 1   &&  async_wattage > c_wattage_max  ) {
		if (  ((unsigned long)(millis_now - millis_last_heatpump_off) > POWERON_HIGHTIME )	||	(async_wattage > c_wattage_max*3)) {
			millis_last_heatpump_on = millis_now;
			heatpump_state = 0;
			LSCint = LSCint_protective;
			halifise();
			lastStopCauseTxt = ("P.WtMax:") + String(async_wattage);
			PrintSS(lastStopCauseTxt);
		}
	}
		
	//-------------------check cycle
	if(  ((unsigned long)(millis_now - millis_prev) > millis_cycle )  ||  (millis_prev == 0) ) {
		millis_prev = millis_now;
		GetTemperatures();  //      wdt_reset here due to 85.0'C filtration
		SaveSetpointEE();
		pr_low_state_anal  	= analogRead(PR_LOW);	//
		pr_high_state_anal  	= analogRead(PR_HIGH);	//shotrcut test shows 993-994 for analogRead (10.4ma)
		if (pr_low_state_anal > 200) {
			pr_low_state_bool = 1;
		} else {
			pr_low_state_bool = 0;
		}
		if (pr_high_state_anal > 200) {
			pr_high_state_bool = 1;
		} else {
			pr_high_state_bool = 0;
		}	
		//--------------------important logic
		//check T sensors
		if ( errorcode == ERR_OK ) {
			if (TbeE == 1 	&& Tbe == -127 )  	{errorcode = ERR_T_SENSOR; outString = F("E.Tbe");}
			if (TaeE == 1 	&& Tae == -127 )  	{errorcode = ERR_T_SENSOR; outString = F("E.Tae");}
			if (TciE == 1 	&& Tci == -127 )	{errorcode = ERR_T_SENSOR; outString = F("E.Tci");}
			if (TcoE == 1 	&& Tco == -127 )  	{errorcode = ERR_T_SENSOR; outString = F("E.Tco");} 
			if (TbcE == 1 	&& Tbc == -127 )  	{errorcode = ERR_T_SENSOR; outString = F("E.Tbc");}
			if (TacE == 1 	&& Tac == -127 )  	{errorcode = ERR_T_SENSOR; outString = F("E.Tac");}
			if (ThiE == 1 	&& Thi == -127 )  	{errorcode = ERR_T_SENSOR; outString = F("E.Thi");}
			if (ThoE == 1 	&& Tho == -127 )  	{errorcode = ERR_T_SENSOR; outString = F("E.Tho");}
			//if (TsgE == 1 	&& Tsg == -127 )  	{errorcode = ERR_T_SENSOR; outString = F("E.Tsg");}
			//if (TslE == 1 	&& Tsl == -127 )  	{errorcode = ERR_T_SENSOR; outString = F("E.Tsl");}
			//if (TbvE == 1 	&& Tbv == -127 )  	{errorcode = ERR_T_SENSOR; outString = F("E.Tbv");}
			//if (TsucE == 1 	&& Tsuc == -127 )  	{errorcode = ERR_T_SENSOR; outString = F("E.Tsuc");}
			if (Ts1E == 1 	&& Ts1 == -127 )  	{errorcode = ERR_T_SENSOR; outString = F("E.Ts1");}
			if (Ts2E == 1	&& Ts2 == -127 )  	{errorcode = ERR_T_SENSOR; outString = F("E.Ts2");}
			if (TcrcE == 1 	&& Tcrc == -127 )  	{errorcode = ERR_T_SENSOR; outString = F("E.Tcrc");}
			if (TregE == 1 	&& Treg == -127 ) 	{errorcode = ERR_T_SENSOR; outString = F("E.Treg");}
			
			if (errorcode == ERR_T_SENSOR){
				//PrintSS(String(outString));
				t_sensorErrString = String(outString);	
				//printed to console below
			}
		}
		
		//auto-clean sensor error on sensor appears
		// add 1xor enable here!
		if ( ( errorcode == ERR_T_SENSOR )     && (   	((TciE == 1 	&& Tci != -127 )  	||	(TciE	^1)) 	&&	
								((TcoE == 1 	&& Tco != -127 )  	||	(TcoE	^1)) 	&&	
								((TbeE == 1 	&& Tbe != -127 )  	||	(TbeE	^1)) 	&&	
								((TaeE == 1 	&& Tae != -127 )  	||	(TaeE	^1)) 	&&	
								//((TsgE == 1 	&& Tsg != -127 )  	||	(TsgE	^1)) 	&&	
								//((TslE == 1 	&& Tsl != -127 )  	||	(TslE	^1)) 	&&	
								//((TbvE == 1 	&& Tbv != -127 )  	||	(TbvE	^1)) 	&&	
								//((TsucE == 1 	&& Tsuc != -127 )  	||	(TsucE	^1)) 	&&	
								((Ts1E == 1 	&& Ts1 != -127 )  	||	(Ts1E	^1)) 	&&	
								((Ts2E == 1	&& Ts2 != -127 )  	||	(Ts2E ^1)) 	&&	
								((TcrcE == 1 	&& Tcrc != -127 )  	||	(TcrcE	^1)) 	&&	
								((TregE == 1 	&& Treg != -127 )  	||	(TregE	^1)) 	&&	
								((TacE == 1 	&& Tac != -127 )  	||	(TacE	^1)) 	&&	
								((TbcE == 1 	&& Tbc != -127 )  	||	(TbcE	^1)) 	&&	
								((ThoE == 1 	&& Tho != -127 )  	||	(ThoE	^1)) 	&&	
								((ThiE == 1 	&& Thi != -127 )  	||	(ThiE	^1)) 	)) {
											errorcode = ERR_OK;
											PrintSSch(IDX_OK_ETSENS);
											sequential_errors = 0;
											t_sensorErrString = "";
		}
		
		//check pressure sensors
		//auto-clean prev. errors first
		if ( errorcode == ERR_P_LO ) {
			if (pr_low_state_bool == 1) 	{
				errorcode = ERR_OK; 
				PrintSSch(IDX_OK_PRCOLD);
			}
		}
		if ( errorcode == ERR_P_HI ) {
			if (pr_high_state_bool == 1) 	{
				errorcode = ERR_OK; 
				PrintSSch(IDX_OK_PRHOT);
			}
		}

		
		//recheck, if another sensor
		if ( errorcode == ERR_OK ) {
			if (pr_low_state_bool == 0) 	{errorcode = ERR_P_LO;}	//for PrintSS scroll down
			if (pr_high_state_bool == 0)	{errorcode = ERR_P_HI;} //for PrintSS scroll down
		}
			
		//-------------- EEV cycle
		/*
		//v1 algo
		if ( EEV_apulses == 0 )	{
			if ( 	((async_wattage < c_workingOK_wattage_min) && ((unsigned long)(millis_now - millis_eev_last_close) > EEV_CLOSEEVERY))		||  millis_eev_last_close == 0  ){
				PrintSS("EEV: FULL closing");
				if ( millis_eev_last_close != 0 ) {
					EEV_apulses =  -(EEV_cur_pos + EEV_CLOSE_ADD_PULSES);
				} else {
					EEV_apulses =  -(EEV_MAXPULSES + EEV_CLOSE_ADD_PULSES);
				}
				EEV_adonotcare = 1;
				EEV_fast = 1;
				//delay(EEV_STOP_HOLD);
				millis_eev_last_close = millis_now;
			} else if (errorcode != 0 || async_wattage <= c_workingOK_wattage_min) {		//err or sleep
				PrintSS("EEV: err or sleep");
				if (EEV_cur_pos <= 0 && EEV_OPEN_AFTER_CLOSE != 0) {				//set waiting pos
					EEV_apulses = +EEV_OPEN_AFTER_CLOSE;
					EEV_adonotcare = 0;
					EEV_fast = 1;
				}
				if (EEV_cur_pos > 0  && EEV_cur_pos != EEV_OPEN_AFTER_CLOSE && EEV_cur_pos <= EEV_MAXPULSES) {	//waiting pos. set
					PrintSS("EEV: close");
					EEV_apulses =  -(EEV_cur_pos + EEV_CLOSE_ADD_PULSES);
					EEV_adonotcare = 1;
					EEV_fast = 1;
				}
			} else if (errorcode == 0 && async_wattage > c_workingOK_wattage_min) {
				T_EEV_dt = Tae.T - Tbe.T;
				PrintSS("EEV: driving " + String(T_EEV_dt));
				if (EEV_cur_pos <= 0){
					PrintSS("EEV: full close protection");	
					if (EEV_OPEN_AFTER_CLOSE != 0) {				//full close protection
						EEV_apulses = +EEV_OPEN_AFTER_CLOSE;
						EEV_adonotcare = 0;
						EEV_fast = 1;
					}
				} else if (EEV_cur_pos > 0) {
					if (T_EEV_dt  < (T_EEV_setpoint - EEV_EMERG_DIFF) ) {				//emerg!
						PrintSS("EEV: emergency closing!");
						EEV_apulses = -EEV_EMERG_STEPS;
						EEV_adonotcare = 0;
						EEV_fast = 1;
					} else if (T_EEV_dt  < T_EEV_setpoint) {					//too
						PrintSS("EEV: closing");
						//EEV_apulses = -EEV_NONPRECISE_STEPS;
						EEV_apulses = -1;
						EEV_adonotcare = 0;
						EEV_fast = 0;
					} else if (T_EEV_dt  > T_EEV_setpoint + EEV_HYSTERESIS + EEV_PRECISE_START) {	//very
						PrintSS("EEV: fast opening");
						//EEV_apulses =  +EEV_NONPRECISE_STEPS;
						EEV_apulses =  +1;
						EEV_adonotcare = 0;
						EEV_fast = 1;
					} else if (T_EEV_dt > T_EEV_setpoint + EEV_HYSTERESIS) {			//too
						PrintSS("EEV: opening");
						EEV_apulses =  +1;
						EEV_adonotcare = 0;
						EEV_fast = 0;
					} else if (T_EEV_dt  > T_EEV_setpoint) {					//ok
						PrintSS("EEV: OK");
						//
					}
				}
				off_EEV();
			} 
			
		}
		*/
		//v1.2 algo: reopen added
		#ifndef NO_EEV
			if ( EEV_manual == 0 && errorcode == 0 && async_wattage >= c_workingOK_wattage_min && EEV_cur_pos > 0 )	{
				T_EEV_dt = Tae - Tbe;
				#ifdef EEV_DEBUG
					PrintSS("EEV Td: " + String(T_EEV_dt));
				#endif
				if ( EEV_apulses >= 0 && EEV_cur_pos >= EEV_MINWORKPOS)	{
					if (T_EEV_dt  < (T_EEV_setpoint - EEV_EMERG_DIFF) ) {				//emerg!
						#ifdef EEV_DEBUG
							PrintSS(F("EEV: 1 emergency closing!"));
						#endif
						EEV_apulses = -1;
						EEV_adonotcare = 0;
						EEV_fast = 1;
					} else if (T_EEV_dt  < T_EEV_setpoint) {					//too
						#ifdef EEV_DEBUG
							PrintSS(F("EEV: 2 closing"));
						#endif
						//EEV_apulses = -EEV_NONPRECISE_STEPS;
						EEV_apulses = -1;
						EEV_adonotcare = 0;
						EEV_fast = 0;
					}
					//faster open when needed, condition copypasted (see EEV_apulses <= 0)
					if (T_EEV_dt  > T_EEV_setpoint + EEV_HYSTERESIS + EEV_PRECISE_START) {		//very
						#ifdef EEV_DEBUG
							PrintSS(F("EEV: 3 enforce faster opening"));
						#endif
						//EEV_apulses =  +EEV_NONPRECISE_STEPS;
						//EEV_apulses =  +1;
						EEV_adonotcare = 0;
						EEV_fast = 1;
					}
				}
				if ( EEV_apulses <= 0 )	{
					
					if ( EEV_must_reopen_flag == 1 && (T_EEV_dt > T_EEV_setpoint + EEV_HYSTERESIS)  && ((unsigned long)(millis_now - millis_eev_minworkpos_time) > EEV_REOPENMINTIME)   &&  (millis_eev_last_work <	millis_eev_minworkpos_time)  ) {	//reopen
						EEV_must_reopen_flag = 0;
						EEV_apulses = EEV_reopen_pos - EEV_cur_pos;
						EEV_adonotcare = 0;
						EEV_fast = 1;
						#ifdef EEV_DEBUG
							PrintSS(F("EEV: 14 reopening last"));
							PrintSS(String(EEV_apulses));
							PrintSS(String(millis_now));
							PrintSS(String(millis_eev_minworkpos_time));
							PrintSS(String(millis_eev_last_work));
						#endif
					} else if (T_EEV_dt  > T_EEV_setpoint + EEV_HYSTERESIS + EEV_PRECISE_START) {	//very
						#ifdef EEV_DEBUG
							PrintSS(F("EEV: 4 fast opening"));
						#endif
						//EEV_apulses =  +EEV_NONPRECISE_STEPS;
						EEV_apulses =  +1;
						EEV_adonotcare = 0;
						EEV_fast = 1;
					} else if (T_EEV_dt > T_EEV_setpoint + EEV_HYSTERESIS) {			//too
						#ifdef EEV_DEBUG
							PrintSS(F("EEV: 5 opening"));
						#endif
						EEV_apulses =  +1;
						EEV_adonotcare = 0;
						EEV_fast = 0;
					} else if (T_EEV_dt  > T_EEV_setpoint) {					//ok
						#ifdef EEV_DEBUG
							PrintSS(F("EEV: 6 OK"));
						#endif
						//
					}
					//faster closing when needed, condition copypasted (see EEV_apulses >= 0)
					if (T_EEV_dt  < (T_EEV_setpoint - EEV_EMERG_DIFF) ) {				//emerg!
						#ifdef EEV_DEBUG
							PrintSS(F("EEV: 7 enforce faster closing!"));
						#endif
						//EEV_apulses = -EEV_EMERG_STEPS;
						EEV_adonotcare = 0;
						EEV_fast = 1;
					}
				}
				off_EEV();
			}
			
			if ( EEV_manual == 0 && EEV_apulses == 0 )	{
				if ( 	((async_wattage < c_workingOK_wattage_min) && ((unsigned long)(millis_now - millis_eev_last_close) > EEV_CLOSEEVERY))		||  millis_eev_last_close == 0  ){	//close every 24h by default
					#ifdef EEV_DEBUG
						PrintSS(F("EEV: 10 FULL closing"));
					#endif
					if ( millis_eev_last_close != 0 ) {
						EEV_apulses =  -(EEV_cur_pos + EEV_CLOSE_ADD_PULSES);
					} else {
						EEV_apulses =  -(EEV_MAXPULSES + EEV_CLOSE_ADD_PULSES);
					}
					EEV_adonotcare = 1;
					EEV_fast = 1;
					//delay(EEV_STOP_HOLD);
					millis_eev_last_close = millis_now;
				}
				else if (errorcode != 0 || async_wattage < c_workingOK_wattage_min) {		//err or sleep
					if (EEV_cur_pos > 0  && EEV_cur_pos > EEV_OPEN_AFTER_CLOSE) {		//waiting pos. set
						EEV_reopen_pos		= EEV_cur_pos;				//reopen pos. set
						EEV_must_reopen_flag	= 1;
						millis_eev_last_work	= millis_now;
						#ifdef EEV_DEBUG
							PrintSS(F("EEV: 11 close before open"));
						#endif
						EEV_apulses =  -(EEV_cur_pos + EEV_CLOSE_ADD_PULSES);
						EEV_adonotcare = 1;
						EEV_fast = 1;
					}
				}
				off_EEV();
			}
			if ( EEV_manual == 0 && EEV_apulses == 0 && async_wattage < c_workingOK_wattage_min && EEV_cur_pos < EEV_OPEN_AFTER_CLOSE) {
				#ifdef EEV_DEBUG
					PrintSS(F("EEV: 12 full close protection"));
				#endif
				if (EEV_OPEN_AFTER_CLOSE != 0) {				//full close protection
					EEV_apulses = EEV_OPEN_AFTER_CLOSE - EEV_cur_pos;
					EEV_adonotcare = 0;
					EEV_fast = 1;
				}
				off_EEV();
			}
			if ( EEV_manual == 0 && EEV_apulses == 0 && async_wattage >= c_workingOK_wattage_min && EEV_cur_pos < EEV_MINWORKPOS) {
				#ifdef EEV_DEBUG
					PrintSS(F("EEV: 13 open to work"));	
				#endif
				if (EEV_MINWORKPOS != 0) {			
					EEV_apulses = EEV_MINWORKPOS - EEV_cur_pos;
					EEV_adonotcare = 0;
					EEV_fast = 1;
					//millis_eev_minworkpos_time = millis_now;
				}
				off_EEV();
			}
			if (EEV_cur_pos < EEV_MINWORKPOS) {	//for reopen
				millis_eev_minworkpos_time = millis_now;
			}
			if ( EEV_manual == 0 && EEV_apulses == 0 && EEV_fast == 1 ) {//just for LED
				EEV_fast = 0;
			}
			if (	((unsigned long)(millis_now - millis_eev_last_on) > 10000)  ||  millis_eev_last_on == 0 ) {
				//PrintSS("EEV: ON/OFF");
				on_EEV();
				//delay(30);
				//off_EEV();	//off_EEV called somewhere else takes care of it
				millis_eev_last_on  =  millis_now;
			}
			//-------------- EEV cycle END
		#endif
		#ifndef EEV_ONLY
			//process heatpump crankcase heater
			if (TcrcE == 1) {
				if ( Tcrc < cT_crc_heat_threshold   &&   crc_heater_state == 0   &&   Tcrc != -127) {
					crc_heater_state = 1;
				} else if (Tcrc >= cT_crc_heat_threshold  && crc_heater_state == 1) {
					crc_heater_state = 0;
				} else if (Tcrc == -127) {
					crc_heater_state = 0;
				}
				halifise();
			}
							
			//main logic
			if (_1st_start_sleeped == 0) {
				//enable hot WP immidiately
				if (hotside_circle_state  == 0){
					millis_last_hotWP_off = millis_now;
					hotside_circle_state  = 1;
				}
				//_1st_start_sleeped = 1;
				if ( (millis_now < poweron_pause) && (_1st_start_sleeped == 0) ) {
					outString = String(((poweron_pause-millis_now))/1000);
					//PrintSS("Wait: " + outString + " s.");
					lastStartMsgTxt = "StCntd:" + outString; //start countdown, max 5 numerical places
					fl_printSS_lastStartMsgTxt = 1;
					//PrintSS(lastStartMsgTxt);
					//return;
				} else {
					_1st_start_sleeped = 1;
					lastStopCauseTxt="";
					lastStartMsgTxt="";
				}
			}
			
			//process_heatpump:
			//start if
			//    (last_on > N or not_started_yet)
			//    and (no errors)
			//    and (t hot out < t target)
			//    and (t hot out < t hot max)
			//    and (t hot in < t hot max)
			//    and (crc t > min'C)
			//    and (crc t < max'C)
			//    and (t watertank < target)
			//    and (t after evaporator > after evaporator min)
			//    and (t cold in > cold min)
			//    and (t cold out > cold min)
			if (heatpump_state == 0 &&	errorcode == ERR_T_SENSOR) {
				lastStartMsgTxt = t_sensorErrString;
				//fl_printSS_lastStartMsgTxt = 1;
			}
			
			if (heatpump_state == 0 && errorcode == ERR_P_LO ) {
				lastStartMsgTxt = F("E.PresCold");
			}
			
			if (heatpump_state == 0 && errorcode == ERR_P_HI ) {
				lastStartMsgTxt = F("E.PresHot");
			}

			if (heatpump_state == 0 &&	errorcode == ERR_OK 	&& 	_1st_start_sleeped == 1) {
				i = 0;
				#ifdef SETPOINT_THI
				if ( Thi < T_setpoint )  								{i+=1;} else { lastStartMsgTxt = F("#Thi>Setp."); }	//or1	//Thi = warm floor heat pump
				#endif
				#ifdef SETPOINT_TS1
					if ( Ts1 < T_setpoint )  								{i+=1;} else { lastStartMsgTxt = F("#Ts1>Setp."); }	//or1	//Ts1 = tank heater
				#endif
				//2	wait cold circe if needed
				if ( coldside_circle_state  == 1 && ((unsigned long)(millis_now - millis_last_coldWP_off) > COLDCIRCLE_PREPARE) ){
					i+= 1;
				//only if hot runned and T < setpoint
				} else if ((coldside_circle_state  == 0) && (hotside_circle_state  == 1) && ((unsigned long)(millis_now - millis_last_hotWP_off) > HOTCIRCLE_CHECK_PREPARE) ) {
					#ifdef SETPOINT_THI
					if ( Thi < T_setpoint ) 	{
					#endif
					#ifdef SETPOINT_TS1
					if ( Ts1 < T_setpoint )  	{
					#endif
						lastStartMsgTxt = F("#CPpStart");
						millis_last_coldWP_off = millis_now;
						coldside_circle_state  = 1;
						fl_printSS_lastStartMsgTxt = 1;
						//PrintSS(lastStartMsgTxt);
					}
				} else if (coldside_circle_state  == 1) {
					lastStartMsgTxt = "#CPp:" + String( (COLDCIRCLE_PREPARE -(unsigned long)(millis_now - millis_last_coldWP_off))/1000 );
				}
				//3	wait hot circe if needed
				#ifdef SETPOINT_THI
					if ((hotside_circle_state  == 1) && ((unsigned long)(millis_now - millis_last_hotWP_off) > HOTCIRCLE_CHECK_PREPARE)	)	{	
						i+=1;
					} else if (hotside_circle_state  == 1) {	//waiting for T stabilisation
						lastStartMsgTxt = "#HotPrp:" + String( (HOTCIRCLE_CHECK_PREPARE -(unsigned long)(millis_now - millis_last_hotWP_off))/1000 );
					} else if (hotside_circle_state  == 0) {	//sleeping, hot CP off, waiting for next check cycle
						lastStartMsgTxt = "#HotSlp:" + String( (HOTCIRCLE_START_EVERY -(unsigned long)(millis_now - millis_last_hotWP_on))/1000 );
					}
				#else ifdef SETPOINT_TS1
					i+=1;
				#endif
				//4	countdown, compressor min. cycle
				if (((unsigned long)(millis_now - millis_last_heatpump_on) > mincycle_poweroff)   ||   (millis_last_heatpump_on == 0)  )  	{	
					i+=1;
				} else {
					if (millis_last_heatpump_on != 0){
						lastStartMsgTxt = "#HPSlp:" + String( (mincycle_poweroff -(unsigned long)(millis_now - millis_last_heatpump_on))/1000 );
					}
				}
				
				if ( (TcrcE == 1 	&& Tcrc > cT_crc_min)  			||	(TcrcE^1))	{i+=1;} else { lastStartMsgTxt = F("#CaseCold"); }	//5
				if ( (TaeE == 1 	&& Tae > cT_coldref_min)		||	(TaeE^1))	{i+=1;} else { lastStartMsgTxt = F("#Tae<RefMin"); }	//6
				if ( (TbeE == 1 	&& Tbe > cT_coldref_min)		||	(TbeE^1))	{i+=1;} else { lastStartMsgTxt = F("#Tbe<RefMin"); }	//7
				if ( (TciE == 1 	&& Tci > cT_cold_min)  			||	(TciE^1))	{i+=1;} else { lastStartMsgTxt = F("#Tci<ColdMin"); }	//8
				if ( (TcoE == 1 	&& Tco > cT_cold_min) 			||	(TcoE^1))	{i+=1;} else { lastStartMsgTxt = F("#Tco<ColdMin"); }	//9
				if ( (ThoE == 1 	&& Tho 	< cT_hot_max)			||	(ThoE^1))	{i+=1;} else { lastStartMsgTxt = F("#Tho>Max"); }	//10
				if ( (ThiE == 1 	&& Thi 	< cT_hot_max)			||	(ThiE^1))	{i+=1;} else { lastStartMsgTxt = F("#Thi>Max"); }	//11
				//t1_crc > t2_cold_in   && ???
				if ( (TcrcE == 1 	&& Tcrc < cT_crc_max)  			||	(TcrcE^1))	{i+=1;} else { lastStartMsgTxt = F("#CaseHot"); }	//12
				if ( (TbcE == 1 	&& Tbc < cT_before_condenser_max) 	||	(TbcE^1))	{i+=1;} else { lastStartMsgTxt = F("#Tbc>Max"); }	//13
				//if ( (TregE == 1 	&& Treg > cT_crc_min)  			||	(TregE^1))	{i+=1;} else { lastStartMsgTxt = F("RegCold"); }	//14
				//if ( (TsucE == 1 	&& Tsuc > cT_coldref_min)		||	(TsucE^1))	{i+=1;} else { lastStartMsgTxt = F("Suc<CRMin"); }	//15
				if (i == 13) {
							//PrintSS(F("HP Started"));
							lastStartMsgTxt = F("HP_Started");
							fl_printSS_lastStartMsgTxt = 1;
							millis_last_heatpump_off = millis_now;
							heatpump_state = 1;
							lastStopCauseTxt = "";
							//lastStartMsgTxt = "";
				} else if (i < 13){
					//"waiting for something" state, do nothing here
				} else {
					//lastStartMsgTxt = F("UErr:1897");
					//PrintSS(lastStartMsgTxt);
				}
			}
			
			//
			
			//stop if
			//    ( (last_off > N) and (t watertank > target) )
			#ifdef SETPOINT_THI
			if ( heatpump_state == 1     &&     ((unsigned long)(millis_now - millis_last_heatpump_off) > mincycle_poweron)    &&    (Thi > T_setpoint) &&  errorcode == ERR_OK) {//or Ts1, if tank heater
			#endif
			#ifdef SETPOINT_TS1
			if ( heatpump_state == 1     &&     ((unsigned long)(millis_now - millis_last_heatpump_off) > mincycle_poweron)    &&    (Ts1 > T_setpoint) &&  errorcode == ERR_OK) {//or Thi, if default warm floor heat pump
			#endif
				millis_last_heatpump_on = millis_now;
				heatpump_state = 0;
				LSCint = LSCint_normal;
				lastStopCauseTxt=F("Normal_stop");
				fl_printSS_lastStopCauseTxt = 1;
				//PrintSS(lastStopCauseTxt);
			}

			//process_hot_side_pump:
			//start if (heatpump_enabled)
			//stop if (heatpump_disabled and (t hot out or in < t target + heat delta min) )
			if (  ((heatpump_state == 1)   &&  (hotside_circle_state  == 0) ) 	|| 	((_1st_start_sleeped == 0 ) && (hotside_circle_state  == 0))	){
				PrintSSch(IDX_HWPON);
				millis_last_hotWP_off = millis_now;
				hotside_circle_state  = 1;
			}
			#ifdef SETPOINT_THI
				if (  (heatpump_state == 0)   &&  (hotside_circle_state  == 0)  && ((unsigned long)(millis_now - millis_last_hotWP_on) > HOTCIRCLE_START_EVERY)	) {    //process START_EVERY for hot side
					millis_last_hotWP_off = millis_now;
					hotside_circle_state  = 1;
					//PrintSS(F("HWP ON by startevery"));
					lastStartMsgTxt = F("HWP_ON_by_ev");
					fl_printSS_lastStartMsgTxt = 1;
				}
			#endif
			
			if (  (heatpump_state == 0)        &&    (hotside_circle_state  == 1) ) {
				if (	( (unsigned long)(millis_now - millis_last_heatpump_on) > deffered_stop_hotcircle) 	|| 	millis_last_heatpump_on == 0) 	{ //deffered stop aftret heat pump stop and correct processing of 1st start, 1st_start sleeped flag not used - there's another logic
					/*
					//useful for tank heater with Ts1 as setpont control and large intermediate water reservoir
					if ( 	(ThoE == 1 && Tho < (Ts1 + cT_hotcircle_delta_min))	||
						(ThiE == 1 && Thi < (Ts1 + cT_hotcircle_delta_min))	) {
						PrintSS(F("Hot CP OFF 1"));
						millis_last_hotWP_on = millis_now;
						hotside_circle_state  = 0;
					} else {
						PrintSS(F("Hot CP OFF 2"));
						millis_last_hotWP_on = millis_now;
						hotside_circle_state  = 0;
					}
					*/
					if ( (unsigned long)(millis_now - millis_last_hotWP_off) > HOTCIRCLE_STOP_AFTER) {	//and START_EVERY processing
						#ifdef SETPOINT_THI
						if ( Thi > T_setpoint ) 	{
						#endif
						#ifdef SETPOINT_TS1
						if ( Ts1 > T_setpoint )  	{
						#endif
							//PrintSS(F("HWP OFF"));
							lastStartMsgTxt = F("HWP_OFF");
							fl_printSS_lastStartMsgTxt = 1;
							millis_last_hotWP_on = millis_now;
							hotside_circle_state  = 0;
						}
					}
				}
			}
			
			//heat if we can, just in case, ex. if lost power, usefull for tank heater with large intermediate water reservoir
			/*
			if ( (hotside_circle_state  == 0) && 
				( ThoE == 1 && Tho > (Ts1 + cT_hotcircle_delta_min)  )  	||
				( ThiE == 1 && Thi > (Ts1 + cT_hotcircle_delta_min)  )  ) 	{
					PrintSS(F("Hot WP ON"));
					hotside_circle_state  = 1;
			}
			*/
			
			//process_cold_side_pump:
			//start if (heatpump_enabled)
			//stop if (heatpump_disbled)
			//start if tci < cold_min
			if (  (heatpump_state == 1)   &&  (coldside_circle_state  == 0)  ) {
				//PrintSS(F("CWP_ON"));
				millis_last_coldWP_off = millis_now;
				coldside_circle_state  = 1;
			}
			
			if (	(heatpump_state == 0)   	&&	(TciE == 1) 	&& 	(Tci > -127.0) 	&& 	(Tci < cT_cold_min) 	&& 	(coldside_circle_state  == 0)	) {
				//PrintSS(F("CWP ON by ColdMin"));
				lastStartMsgTxt = F("CWP_ON_CoMin");
				fl_printSS_lastStartMsgTxt = 1;
				millis_last_coldWP_off = millis_now;
				coldside_circle_state  = 1;
			}
			
			if (  	(heatpump_state == 0)   	&&  	(coldside_circle_state  == 1)	)	{	//is on
				if ( (TciE == 1 	&& Tci > cT_cold_min)  		||	(TciE^1))	{ 	//does not overfrozen
					//next: deal with unstable env. to prevent false starts (water tank with dynamic flows, maybe air heating): stop CWP while waiting period if false start
					//stop if T>S OR if not needed by prepare
					#ifdef SETPOINT_THI
					if (    ( Thi > T_setpoint )  || ((unsigned long)(millis_now - millis_last_coldWP_off) > (COLDCIRCLE_PREPARE*2)) 	) {
					#endif
					#ifdef SETPOINT_TS1
					if (	( Ts1 > T_setpoint )  || ((unsigned long)(millis_now - millis_last_coldWP_off) > (COLDCIRCLE_PREPARE*2))	) {
					#endif
						//PrintSS(F("CWP_OFF"));
						coldside_circle_state  = 0;
					}
				}
			}

			//protective_cycle:
			//stop if
			//      (error)
			//      (t hot out > hot max)
			//      (t hot in  > hot max)
			//      (crc t > max'C)
			//      or (t after evaporator < after evaporator min)
			//      or (t cold in < cold min)
			//      or (t cold out < cold min)
			//      
			if (  heatpump_state == 1   &&  errorcode == ERR_OK ){
				if (ThoE 	== 1 	&&	Tho 	> cT_hot_max)			{heatpump_state = 0; 	lastStopCauseTxt = F("P.Tho");		}
				if (ThiE 	== 1 	&&	Thi 	> cT_hot_max)			{heatpump_state = 0; 	lastStopCauseTxt = F("P.Thi");		}
				if (TcrcE 	== 1 	&&	Tcrc	> cT_crc_max)   		{heatpump_state = 0; 	lastStopCauseTxt = F("P.Tcrc"); 	}
				if (TaeE 	== 1 	&& 	Tae 	< cT_coldref_min)		{heatpump_state = 0; 	lastStopCauseTxt = F("P.Tae"); 		}
				if (TbeE 	== 1 	&& 	Tbe 	< cT_before_evap_work_min) 	{heatpump_state = 0; 	lastStopCauseTxt = F("P.Tbe");		}
				//if (TsucE 	== 1 	&& 	Tsuc 	< cT_coldref_min)		{heatpump_state = 0; 	lastStopCauseTxt = F("P.Tsuc");		}
				if (TbcE 	== 1 	&& 	Tbc 	> cT_before_condenser_max)	{heatpump_state = 0; 	lastStopCauseTxt = F("P.Tbc");		}
				if (TciE 	== 1 	&& 	Tci 	< cT_cold_min)       		{heatpump_state = 0; 	lastStopCauseTxt = F("P.Tci");		}
				if (TcoE 	== 1 	&& 	Tco 	< cT_cold_min)			{heatpump_state = 0; 	lastStopCauseTxt = F("P.Tco");		}
				if (heatpump_state == 0){
					LSCint = LSCint_protective;
					fl_printSS_lastStopCauseTxt = 1;
					//PrintSS(lastStopCauseTxt);
					millis_last_heatpump_on = millis_now;
				}
			}
			
			//5 minutes workout checks
			//alive_check_cycle_after_5_mins:
			//(old)error if
			//(new)not error, just poweroff all
			//next disabled: issues after a deep freeze, long time needed for stabilisation
			//DISABLED//      or (t cold in - t cold out < t workingok min diff) 
			//DISABLED//      or (t hot out - t hot in < t workingok min diff)
			//      or (crc t < 25'C)
			//      or wattage too low
			
			if (  heatpump_state == 1   &&  ((unsigned long)(millis_now - millis_last_heatpump_off) > 300000)  ) {
				//cold side processing simetimes works incorrectly, after long period of inactivity, due to T inertia on cold tube sensor, commented out
				//if ( ( errorcode == ERR_OK )     &&   (  tr_cold_in - tr_cold_out < cT_workingOK_cold_delta_min ) ) {
				//    errorcode = ERR_COLD_PUMP;
				//}
				//if ( ( errorcode == ERR_OK )     &&   (  Tho.e == 1 && Thi.e == 1 && (Tho.T - Thi.T < cT_workingOK_hot_delta_min )) ) {
				//	errorcode = ERR_HOT_PUMP;
				//}
				if ( ( errorcode == ERR_OK )     &&   (  TcrcE == 1 && Tcrc < cT_workingOK_crc_min )  ) {
					//errorcode = ERR_HEATPUMP;
					millis_last_heatpump_on = millis_now;
					heatpump_state = 0;
					LSCint = LSCint_protective;
					lastStopCauseTxt = F("P.W.TcrcMIN");
					fl_printSS_lastStopCauseTxt = 1;
					//PrintSS(lastStopCauseTxt);
				}
				if ( ( errorcode == ERR_OK )     &&   ( async_wattage < c_workingOK_wattage_min )  ) {
					//errorcode = ERR_WATTAGE;
					millis_last_heatpump_on = millis_now;
					heatpump_state = 0;
					LSCint = LSCint_protective;
					lastStopCauseTxt = F("P.W.wattMIN");
					fl_printSS_lastStopCauseTxt = 1;
					//PrintSS(lastStopCauseTxt);
				}
				//digitalWrite(RELAY_HEATPUMP, heatpump_state);	////!!! old, now halifised
			}

				
			//disable pump by t.sensor error, sequentially
			if ( heatpump_state == 1   &&  errorcode == ERR_T_SENSOR ) {
				sequential_errors += 1;
				if (sequential_errors > MAX_SEQUENTIAL_ERRORS) {
					millis_last_heatpump_on = millis_now;
					heatpump_state = 0;
					LSCint = LSCint_error;
					lastStopCauseTxt = t_sensorErrString;
					fl_printSS_lastStopCauseTxt = 1;
				}
				//PrintSS(t_sensorErrString);
			}
			
			if ( errorcode == ERR_OK ) {	//auto-clean counter just in case
				sequential_errors = 0;
			}
			
			//disable pump by pressure error, immediately
			if ( heatpump_state == 1   &&  ( errorcode == ERR_P_HI || errorcode == ERR_P_LO ) ) {
				millis_last_heatpump_on = millis_now;
				heatpump_state = 0;
				if (errorcode == ERR_P_HI) {
					lastStopCauseTxt = F("E.PressHot");				
				} else if (errorcode == ERR_P_LO) {
					lastStopCauseTxt = F("E.PressCold");
				}
				LSCint = LSCint_error;
				fl_printSS_lastStopCauseTxt = 1;
				//PrintSS(lastStopCauseTxt);
			}

			//!!! self-test
			///heatpump_state = 1;
			
			halifise();
			
			if (errorcode == ERR_T_SENSOR) {
				PrintSS(t_sensorErrString);
			}
			
			if (fl_printSS_lastStartMsgTxt == 1){
				PrintSS(lastStartMsgTxt);
				fl_printSS_lastStartMsgTxt = 0;
			}
			
			if (fl_printSS_lastStopCauseTxt == 1){
				PrintSS(lastStopCauseTxt);
				fl_printSS_lastStopCauseTxt = 0;
			}
		#endif
		
		//process errors
		//beep N times error
		if ( errorcode != ERR_OK ) {
			LED_OK_state		= 0;
			LED_ERR_state		= 1;
			if (  ((unsigned long)(millis_now - millis_notification) > millis_notification_interval)  ||  millis_notification == 0 ) {
				millis_notification = millis_now;
				outString = F("Err: ");
				PrintSS_SaI(errorcode);
				for ( i = 0; i < errorcode; i++) {
					LED_ERR_state		= 0;
					halifise();
					analogWrite(speakerOut, 10);  	delay (500);      
					LED_ERR_state		= 1;
					halifise();
					analogWrite(speakerOut, 0);	delay (500);
				}
			}
		} else {
			LED_OK_state		= 1;
			LED_ERR_state		= 0;
			halifise();
		}
	}
	
	if (Serial.available() > 0) {
		inChar = Serial.read();
		if ( inChar == 0x1B ) {
			skipchars_local += 3;
			inChar = 0x00;
			millis_escinput_local = millis();
		}
		if ( skipchars_local != 0 ) {
			millis_charinput_local = millis();
			if ((unsigned long)(millis_charinput_local - millis_escinput_local) < 16*2 ) {	//2 chars for 2400
				if (inChar != 0x7e) {
					skipchars_local -= 1;
				}
				if (inChar == 0x7e) {
					skipchars_local = 0;
				}
				if (inChar >= 0x30 && inChar <= 0x35) {
					skipchars_local += 1;
				}
				inChar = 0x00;
			} else {
				skipchars_local = 0;
			}
		}
		_ProcessInChar();
	}
	
	if (RS485Serial.available() > 0) {
		//PrintSS("some on 485..");	//!!!debug
		#ifdef RS485_HUMAN
			if (RS485Serial.available()) {
				inChar = RS485Serial.read();
				//RS485Serial.print(inChar);	//!!!debug
				if ( inChar == 0x1B ) {
					skipchars_485 += 3;
					inChar = 0x00;
					millis_escinput_485 = millis();
				}
				if ( skipchars_485 != 0 ) {
					millis_charinput_485 = millis();
					//if (millis_escinput_485 + 2 > millis_charinput_485)
					if ((unsigned long)(millis_charinput_485 - millis_escinput_485) < 16*2 ) {	//2 chars for 2400
						if (inChar != 0x7e) {
							skipchars_485 -= 1;
						}
						if (inChar == 0x7e) {
							skipchars_485 = 0;
						}
						if (inChar >= 0x30 && inChar <= 0x35) {
							skipchars_485 += 1;
						}
						inChar = 0x00;
					} else {
						skipchars_485 = 0;
					}
				}
				_ProcessInChar();
			}
		#endif
        
		#ifdef RS485_JSON
			index = 0;
			while (RS485Serial.available() > 0) { // Don't read unless you know there is data
				if(index < 49) {   //  size of the array minus 1
					inChar = RS485Serial.read(); 	// Read a character
					dataBuf[index] = inChar;      	// Store it
					index++;                     	// Increment where to write next
					dataBuf[index] = '\0';        	// clear next symbol, null terminate the string
					delayMicroseconds(80);       	//80 microseconds - the best choice at 9600, "no answer"disappeared
									//40(20??) microseconds seems to be good, 9600, 49 symbols
									//
				} else {            //too long message! read it to nowhere
					inChar = RS485Serial.read();
					delayMicroseconds(80);
					//break;    //do not break if symbols!!
				}
			}
		
			//!!!debug, be carefull, can cause strange results
			/*
			if (dataBuf[0] != 0x00) {
			PrintSS("-");
			PrintSS(dataBuf);
			PrintSS("-");
			}
			*/
			//or this debug
			/*
			digitalWrite(SerialTxControl, RS485Transmit);
			halifise();
			delay(10);
			RS485Serial.println(dataBuf);
			RS485Serial.flush();
			RS485Serial.println(index);
			*/
			
			//ALL lines must be terminated with \n!
			if ( (dataBuf[0] == hostID) && (dataBuf[1] == devID) ) {             
				//  COMMANDS:
				// G (0x47): (G)et main data
				// TNN.NN (0x54): set aim (T)emperature
				// ENN.NN (0x45): set (E)EV difference aim
				digitalWrite(SerialTxControl, RS485Transmit);
				halifise();
				delay(1);
				//PrintSS(freeMemory());
				outString = "";
				outString = devID;
				outString += hostID;
				outString +=  "A ";  //where A is Answer, space after header
				char *outChar=&outString[0];
				if ( (dataBuf[2] == 0x47 ) ) {
					//PrintSS("G");
					//WARNING: this procedure can cause "NO answer" effect if no or few T sensors connected
					
					//outString = "";
					//if (TsgE)	{	outString += ",\"TSG\":" + String(Tsg);	}
					//if (TslE)	{	outString += ",\"TSL\":" + String(Tsl);	}
					//if (TbvE)	{	outString += ",\"TBV\":" + String(Tbv);	}
					//if (TsucE)	{	outString += ",\"TSUC\":" + String(Tsuc);}
					//RS485Serial.write(outChar);                    	//dirty hack to transfer long string
					//RS485Serial.flush();
					//delay (1);                                      //lot of errors without delay
					outString += "{";
					outString += "\"E1\":" + String(errorcode);  
					if (TciE)	{ 	outString += ",\"TCI\":"; 	ApToOut_D(Tci);	}
					if (TcoE)	{	outString += ",\"TCO\":"; 	ApToOut_D(Tco);	}
					if (TbeE)	{	outString += ",\"TBE\":"; 	ApToOut_D(Tbe);	}
					if (TaeE)	{	outString += ",\"TAE\":"; 	ApToOut_D(Tae);	}
					if (Ts1E)	{	outString += ",\"TS1\":"; 	ApToOut_D(Ts1);	}
					if (Ts2E)	{	outString += ",\"TS2\":"; 	ApToOut_D(Ts2);	}
					if (TcrcE)	{	outString += ",\"TCRC\":"; 	ApToOut_D(Tcrc);}
					if (TregE)	{	outString += ",\"TR\":"; 	ApToOut_D(Treg);}
					RS485Serial.write(outChar);                    	//dirty hack to transfer long string
					RS485Serial.flush();
					delay (1);                                      //lot of errors without delay
					
					outString = "";
					if (TacE)	{	outString += ",\"TAC\":"; 	ApToOut_D(Tac);	}
					if (TbcE)	{	outString += ",\"TBC\":"; 	ApToOut_D(Tbc);	}
					if (ThoE)	{	outString += ",\"THO\":"; 	ApToOut_D(Tho);	}
					if (ThiE)	{	outString += ",\"THI\":"; 	ApToOut_D(Thi);}
					outString += ",\"W1\":";	ApToOut_D(async_wattage);
					outString += ",\"EEVP\":" 	+ String(EEV_cur_pos);
					outString += ",\"EEVA\":"; 	ApToOut_D(T_EEV_setpoint);
					
					#ifndef EEV_ONLY
						outString += ",\"A1\":"; 	ApToOut_D(T_setpoint);  //(A)im (target)
						outString += ",\"RP\":" 	+ String(heatpump_state*RELAY_HEATPUMP);  
						outString += ",\"RH\":" 	+ String(hotside_circle_state*RELAY_HOTSIDE_CIRCLE);                  
						outString += ",\"RC\":" 	+ String(coldside_circle_state*1);  
						outString += ",\"RCRCH\":" 	+ String(crc_heater_state*3);                 
						//if (TregE)	{	outString += ",\"RRH\":" + String(reg_heater_state*4);}                 
						//RS485Serial.write(outChar);                    	//dirty hack to transfer long string
						//RS485Serial.flush();
						//delay (1);                                      //lot of errors without delay
					#endif
					RS485Serial.write(outChar);                    	//dirty hack to transfer long string
					RS485Serial.flush();
					delay (1);                                      //lot of errors without delay
					
					outString = "";
					outString = ",\"LSC\":\"";
					outString += lastStopCauseTxt;
					outString += ("\"");
					//RS485Serial.write(outChar);                    	//dirty hack to transfer long string
					//RS485Serial.flush();
					//delay (1);                                      //lot of errors without delay
					outString += ",\"LSM\":\"";
					outString += lastStartMsgTxt;
					outString += ("\"");
					outString += "}";
					
				} else if ( (dataBuf[2] == 0x54 ) || (dataBuf[2] == 0x45 )) {  //(T)arget or (E)EV target format NN.NN, text
					if ( isDigit(dataBuf[ 3 ]) && isDigit(dataBuf[ 4 ]) && (dataBuf[ 5 ] == 0x2e)  && isDigit(dataBuf[ 6 ]) && isDigit(dataBuf[ 7 ]) && ( ! isDigit(dataBuf[ 8 ])) ) {
						
						analogWrite(speakerOut, 10);
						delay (100); 
						analogWrite(speakerOut, 0);
						
						char * carray = &dataBuf[ 3 ];
						tempdouble = atof(carray);                
						if (dataBuf[2] == 0x54 ){
							if (tempdouble > cT_setpoint_max) {
								outString += "{\"r\":\"too hot!\"}";
							} else if (tempdouble < cT_setpoint_min) {
								outString += "{\"r\":\"too cold!\"}";
							} else {
								T_setpoint = tempdouble;
								_HotWPon_by_Setpoint_update();
								outString += "{\"r\":\"ok, new value: "; 
								ApToOut_D(T_setpoint);
								outString += "\"}";
							}
						}
						if (dataBuf[2] == 0x45 ) {
							if (tempdouble > 10.0) {		//!!!!!!! hardcode !!!
								outString += "{\"r\":\"too hot!\"}";
							} else if (tempdouble < 0.1) {		//!!!!!!! hardcode !!!
								outString += "{\"r\":\"too cold!\"}";
							} else {
								T_EEV_setpoint = tempdouble;
								outString += "{\"r\":\"ok, new EEV value: ";
								ApToOut_D(T_EEV_setpoint);
								outString += "\"}";
							}
						}
					} else {
						outString += "{\"r\":\"NaN, format: NN.NN\"}";
					}
				} else {
					//default, just for example
					outString += "{\"r\":\"no_command\"}";
				}
				//crc.integer = CRC16.xmodem((uint8_t& *) outString, outString.length());
				//outString += (crc, HEX);
				outString += "\n";
				RS485Serial.write(outChar);
			}
			
			index = 0;
			for (i=0;i < (BUFSIZE);i++) {  //clear buffer
				dataBuf[i]=0;
			}
			RS485Serial.flush();
			digitalWrite(SerialTxControl, RS485Receive);
			delay(1);
		#endif
		
		#ifdef RS485_MODBUS
			index = 0;
			z = 0;  //error flag
			while ( 1 == 1 ) {//9600
				//read
				//!!!!!!!
				//Serial.println("-");
				if (RS485Serial.available()) {
					if(index < BUFSIZE) {
						inChar = RS485Serial.read();	 
						//Serial.print(inChar, HEX);
						//Serial.print(" ");
						dataBuf[index] = inChar;      	
						index++;                   	
						dataBuf[index] = '\0';        	
						delayMicroseconds(80);       	//yep, 80, HERE
					} else {
						z = 1;
						while (RS485Serial.available()) {
							inChar = RS485Serial.read();
							delayMicroseconds(1800);
						}
						break;
					}
				} else {
					//Serial.print(".");
					tmic1 = micros();
					for (i = 0; i < 10; i++) {    
						delayMicroseconds(180);
						if (RS485Serial.available()){
							//Serial.print("babaika");
							//Serial.println(i);
							tmic2 = micros();
							break;
						}
						tmic2 = micros();
						if ( (unsigned long)(tmic2 - tmic1) > 1800 ){
							i = 10;
							break;
						}
					}
					if (i == 10 && RS485Serial.available()) {
						z = 2;
						i = 0;
						while (RS485Serial.available()) {
							if (i > 200){ 
								break; 
							}
							inChar = RS485Serial.read();
							delayMicroseconds(1800);
							i++;
						}
						break;
					} else if (!RS485Serial.available()) {
						break;
					} else if (RS485Serial.available()) {
						continue;
					} else {
						//PrintSS(F("e2245"));
					}
				}
			}
			
			
			//check CRC
			if (index < 3) { 
				z+= 10;
			}
			if ( dataBuf[1] == 0x03 && ( (index % 8 ) == 0) && index > 8 ) { //automatic "duplicated message" detector, can be found if lot of T sensors absent and requests are too fast
				index = 8;
			}
				
			crc16 = SEED;
			for (x = 0; x < (index-2); x++) {   
				Calc_CRC(dataBuf[x]); 
			}
			x = dataBuf[index - 2];
			y = dataBuf[index - 1];
			if (( x !=  (crc16 & 0xFF )) || ( y != (crc16 >> 8))) {
				z += 100;
			}
			//PrintSS(F("-----"));
			if ( z != 0 ) {
				//probably another proto
				//PrintSS(F("MmsgERR: "));
				/*Serial.println(z);
				for (x =0; x<index; x++){
					Serial.print(dataBuf[x], HEX);
					Serial.print(" ");
				}
				Serial.println();*/
			}  else {		
				/*PrintSS(F("ModbusMSG: "));
				Serial.println(z);
				for (x =0; x<index; x++){
					Serial.print(dataBuf[x], HEX);
					Serial.print(" ");
				}
				Serial.println();*/
				
				digitalWrite(SerialTxControl, RS485Transmit);
				halifise();
				z = 0;
				if (dataBuf[0] != 0x00 && dataBuf[0] != devID ) {	//will reply to 0x00
					z = 0xFF;
				}
				if (dataBuf[1] != 0x03 && dataBuf[1] != 0x06) { //0x01
					z = 1;
				}
				if (dataBuf[1] == 0x03 && dataBuf[2] != 0x00 && dataBuf[4] != 0x00) { //0x02
					z = 2;
				}
				if (dataBuf[1] == 0x06 && dataBuf[2] != 0x00) { 	//0x02
					z = 2;
				}
				if (dataBuf[1] == 0x06 && dataBuf[3] > MODBUS_MR)  {	//0x03
					z = 3;
				}
				if (dataBuf[1] == 0x03 && dataBuf[5] > MODBUS_MR)  {	//0x05 
					z = 5;
				}
				
				i = 0;
				//dataBuf[i]  =  devID;
				//unchanged! can be devID or 0x00
				i++;
				if (z == 0) {
					//PrintSS(F("ModParse"));
					x = dataBuf[3]; 	//addr	
					y = dataBuf[5];		//num
					if (dataBuf[1]  ==  0x03) {
						//PrintSS(F("F03"));
						dataBuf[i]  = 0x03;
						i++;	
						//the most significant byte is sent first
						dataBuf[i]  =  y*2;
						i++;  // data
						for (u = x; u < (x+y); u++) {
							if (u > MODBUS_MR){
								z = 2;
								break;
							}
							switch (u) {								
								case 0x00:
									Add_Double_To_Buf_IntFract(Tci); //uses dataBuf, i
									break;
								case 0x01:
									Add_Double_To_Buf_IntFract(Tco); //uses dataBuf, i
									break;
								case 0x02:
									Add_Double_To_Buf_IntFract(Tbe); //uses dataBuf, i
									break;
								case 0x03:
									Add_Double_To_Buf_IntFract(Tae); //uses dataBuf, i
									break;
								case 0x04:
									//Add_Double_To_Buf_IntFract(Tsg); //uses dataBuf, i
									dataBuf[i]  = 0;
									i++;
									dataBuf[i]  = 0;
									i++;
									break;
								case 0x05:
									//Add_Double_To_Buf_IntFract(Tsl); //uses dataBuf, i
									dataBuf[i]  = 0;
									i++;
									dataBuf[i]  = 0;
									i++;
									break;
								case 0x06:
									//Add_Double_To_Buf_IntFract(Tbv); //uses dataBuf, i
									dataBuf[i]  = 0;
									i++;
									dataBuf[i]  = 0;
									i++;
									break;
								case 0x07:
									//Add_Double_To_Buf_IntFract(Tsuc); //uses dataBuf, i
									dataBuf[i]  = 0;
									i++;
									dataBuf[i]  = 0;
									i++;
									break;
								case 0x08:
									Add_Double_To_Buf_IntFract(Ts1); //uses dataBuf, i
									break;
								case 0x09:
									Add_Double_To_Buf_IntFract(Ts2); //uses dataBuf, i
									break;
								case 0x0A:
									Add_Double_To_Buf_IntFract(Tcrc); //uses dataBuf, i
									break;
								case 0x0B:
									Add_Double_To_Buf_IntFract(Treg); //uses dataBuf, i
									break;
								case 0x0C:
									Add_Double_To_Buf_IntFract(Tac); //uses dataBuf, i
									break;
								case 0x0D:
									Add_Double_To_Buf_IntFract(Tbc); //uses dataBuf, i
									break;
								case 0x0E:
									Add_Double_To_Buf_IntFract(Tho); //uses dataBuf, i
									break;
								case 0x0F:
									Add_Double_To_Buf_IntFract(Thi); //uses dataBuf, i
									break;
								case 0x10:
									dataBuf[i]  = 0;
									i++;
									dataBuf[i]  = errorcode;
									i++;
									break;
								case 0x11:
									dataBuf[i]  = (int)async_wattage >> 8;
									i++;
									dataBuf[i]  = (int)async_wattage & 0xFF;
									i++;
									break;
								case 0x12:
									dataBuf[i]  = 0;
									i++;
									dataBuf[i]  = 0;
									bitWrite(dataBuf[i], 0, heatpump_state);
									bitWrite(dataBuf[i], 1, hotside_circle_state);
									bitWrite(dataBuf[i], 2, coldside_circle_state);
									bitWrite(dataBuf[i], 3, crc_heater_state);
									//bitWrite(dataBuf[i], 4, reg_heater_state);
									i++;
									break;
								case 0x13:
									Add_Double_To_Buf_IntFract(T_EEV_setpoint); //uses dataBuf, i
									break;
								case 0x14:
									Add_Double_To_Buf_IntFract(T_setpoint); //uses dataBuf, i
									break;
								case 0x15:
									dataBuf[i]  = (int)EEV_cur_pos >> 8;
									i++;
									dataBuf[i]  = (int)EEV_cur_pos & 0xFF;
									i++;
									break;
								case 0x16:
									dataBuf[i]  = lastStopCauseTxt.charAt(0);
									i++;
									dataBuf[i]  = lastStopCauseTxt.charAt(1);
									i++;
									break;
								case 0x17:
									dataBuf[i]  = lastStopCauseTxt.charAt(2);
									i++;
									dataBuf[i]  = lastStopCauseTxt.charAt(3);
									i++;
									break;
								case 0x18:
									dataBuf[i]  = lastStopCauseTxt.charAt(4);
									i++;
									dataBuf[i]  = lastStopCauseTxt.charAt(5);
									i++;
									break;
								case 0x19:
									dataBuf[i]  = lastStopCauseTxt.charAt(6);
									i++;
									dataBuf[i]  = lastStopCauseTxt.charAt(7);
									i++;
									break;
								case 0x1A:
									dataBuf[i]  = lastStopCauseTxt.charAt(8);
									i++;
									dataBuf[i]  = lastStopCauseTxt.charAt(9);
									i++;
									break;
								case 0x1B:
									dataBuf[i]  = lastStopCauseTxt.charAt(10);
									i++;
									dataBuf[i]  = lastStopCauseTxt.charAt(11);
									i++;
									break;
								case 0x1C:
									dataBuf[i]  = lastStartMsgTxt.charAt(0);
									i++;
									dataBuf[i]  = lastStartMsgTxt.charAt(1);
									i++;
									break;
								case 0x1D:
									dataBuf[i]  = lastStartMsgTxt.charAt(2);
									i++;
									dataBuf[i]  = lastStartMsgTxt.charAt(3);
									i++;
									break;
								case 0x1E:
									dataBuf[i]  = lastStartMsgTxt.charAt(4);
									i++;
									dataBuf[i]  = lastStartMsgTxt.charAt(5);
									i++;
									break;
								case 0x1F:
									dataBuf[i]  = lastStartMsgTxt.charAt(6);
									i++;
									dataBuf[i]  = lastStartMsgTxt.charAt(7);
									i++;
									break;
								case 0x20:
									dataBuf[i]  = lastStartMsgTxt.charAt(8);
									i++;
									dataBuf[i]  = lastStartMsgTxt.charAt(9);
									i++;
									break;
								case 0x21:
									dataBuf[i]  = lastStartMsgTxt.charAt(10);
									i++;
									dataBuf[i]  = lastStartMsgTxt.charAt(11);
									i++;
									break;
								default:
									dataBuf[i]  = 0x00;
									i++;
									dataBuf[i]  = 0x00;
									i++;
									break;
							}
						}
					} else if (dataBuf[1]  ==  0x06) {	//de-facto echo
						//PrintSS(F("F06"));
						dataBuf[i]  = 0x06;
						i++;
						dataBuf[i]  = 0x00;
						i++;
						dataBuf[i]  = x;
						i++;

						switch (x) {
							case 0x13:
								//PrintSS(F("06F_EEV_setpoint"));
								IntFract_to_tempdouble(dataBuf[4], dataBuf[5]);
								//Serial.println(tempdouble);
								if (tempdouble > 15.0 || tempdouble < -15.0) {		//incorrectest values filter
									z = 3;
									break;
								}
								T_EEV_setpoint = tempdouble;
								//Serial.println(T_EEV_setpoint);
								Add_Double_To_Buf_IntFract(T_EEV_setpoint); //uses dataBuf, i
								break;
							case 0x14:
								//PrintSS(F("06F_T_setpoint"));
								IntFract_to_tempdouble(dataBuf[4], dataBuf[5]);
								//Serial.println(tempdouble);
								if (tempdouble > cT_setpoint_max || tempdouble < cT_setpoint_min) {	//incorrectest values filter
									z = 3;
									break;
								}
								T_setpoint = tempdouble;
								_HotWPon_by_Setpoint_update();
								//Serial.println(T_setpoint);
								Add_Double_To_Buf_IntFract(T_setpoint); //uses dataBuf, i
								break;
							//case 0x15:
							//	//EEV_cur_pos
							//	break;
							default:
								z = 3;
								break;
						}
					} else {
						PrintSSch(IDX_UNKNF);
						z = 1;
					}
					if (z != 0) {
						i = 1;
						bitWrite(dataBuf[i], 7, 1);
						i++;
						dataBuf[i] = z;
						i++;
					}
					
					crc16 = SEED;
					for (x = 0; x < (i); x++) {   
						Calc_CRC(dataBuf[x]); 
					}
					dataBuf[i] = crc16 & 0xFF;
					i++;
					dataBuf[i] = crc16 >> 8;
					i++;
					
					RS485Serial.write(dataBuf, i);
					RS485Serial.flush();	
					delay (1);              

					//!!! debug
					/*
					for (x = 0; x<i; x++){
						Serial.print(dataBuf[x], HEX);
						Serial.print(" ");
					}
					*/
					/*Serial.println("ModResp");
					for (z = 0; z < i; z++){
						Serial.print(dataBuf[z], HEX);  
						Serial.print(" ");  
						if ( z%50 == 0) Serial.println();
					}
					Serial.println();*/
					
					digitalWrite(SerialTxControl, RS485Receive);
				}
			}
		#endif
	}
}
