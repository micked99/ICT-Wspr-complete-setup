//#pragma GCC diagnostic error "-Wconversion"

/*
   HABalloon by KD2NDR, Miami Florida October 25 2018
   Improvements by YO3ICT, Romania, April-May 2019 
   Improvements By SA6BSS, Sweden, 2020-2026
   You may use and modify the following code to suit
   your needs so long as it remains open source
   and it is for non-commercial use only.
*/

#include <avr/wdt.h>
#include <TimeLib.h>
#include <TinyGPS++.h>
#include <JTEncode.h>
#include <Wire.h>
#include <avr/pgmspace.h>
#include <avr/power.h>

//#define WSPR_FREQ     3570185UL  // <<<<< SET TX FREQUENCY HERE
//#define WSPR_FREQ     7040185UL  // <<<<< SET TX FREQUENCY HERE
//#define WSPR_FREQ     10140285UL // <<<<< SET TX FREQUENCY HERE
//#define WSPR_FREQ     14097190UL // <<<<< SET TX FREQUENCY HERE
//#define WSPR_FREQ     18106200UL  // <<<<< SET TX FREQUENCY HERE
//#define WSPR_FREQ     21096190UL // <<<<< SET TX FREQUENCY HERE  
#define WSPR_FREQ     24926055UL // <<<<< SET TX FREQUENCY HERE       
//#define WSPR_FREQ       28126100UL // <<<<< SET TX FREQUENCY HERE
#define ISM_WSPR_FREQ   13555425UL // = 1,500Hz audio on 13.553.900 Mhz // 13555425UL New correct ism freq
#define WSPR_CTC        1334       // WSPR CTC - 2668 @ 4Mhz //1334 @ 2Mhz //667 @ 1Mhz


// Enumerations
TinyGPSPlus gps;
JTEncode jtencode;

// Global variables
unsigned long freq;
const char call[] = "SA7BSS";      // WSPR Standard callsign -- SA7BSS for bench testing, SA6BSS for flight
char tlm_channel1 = '1';           // Telemetry channel: 0/1/Q
char tlm_channel2 = '1';           // Telemetry channel: 0-9
const uint32_t tcxo  = 26000000UL; // TCXO/crystal frequency in Hz
const bool use180deg = false;      // Hardware config: true = 180° dipole (CLK0+CLK1), false = standard/ground-wire (CLK0)
// ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑ // Edit above settings to match your setup 
char call_telemetry[7];            // WSPR telemetry callsign
char loc_telemetry[5];             // WSPR telemetry locator
char TestGrid[5];                  // Testing grid for Geofence
uint8_t dbm, dbm_telemetry;        // Standard WSPR dbm - coarse altitude & WSPR telemetry dbm
char loc4[5];                      // 4 digit gridsquare locator
long lat, lon, oldlat, oldlon;     // Used for location
uint8_t tx_buffer[255];            // WSPR Tx buffer
uint8_t symbol_count;              // JTencode
uint16_t tone_spacing, CTC;        // JTencode
int alt_meters = 0;
int telemetry_set = 0;
int Sats = 0;
int gps_speed = 0;
volatile bool proceed = false;
int w=1; 
bool Outside = true;             // Geofence flag
//GeoFence grids by Matt Downs - 2E1GYP // Airborne transmissions of this sort are not legal over North Korea and Yemen
const char NoTXGrids[] PROGMEM = {"PN31 PN41 PN20 PN30 PN40 PM29 PM39 PM28 PM38 LK16 LK15 LK14 LK13 LK23 LK24 LK25 LK26 LK36 LK35 LK34 LK33 LK44 LK45 LK46 LK47 LK48 LK58 LK57 LK56 LK55"};

#include "TelemFunctions.h"      // Various telemetry functions
#include "Beep.h"                // beeps and CW
#include "Timing.h"             // Scheduling, set Timing-test.h for testing, it will tx on first 2 min slot after power on, for flight use Timing.h it will tx on the nominal 10 min schedule

// Function Prototype
void wdt_init(void) __attribute__ ((naked, used, section(".init3")));
// Function Implementation
void wdt_init(void)
 {
  MCUSR = 0x00;  //Clear all reset flags
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDP3) | (1 << WDP0);
  return;
 }

void setup()
 {
  cli(); //Disable global interrupts
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (0 << WDIE) | (1 << WDE) | (1 << WDP3) | (1 << WDP0);  // First enable the watchdog timer
  sei(); //Enable global interrupts

  clock_prescale_set(clock_div_2);
  //clock_prescale_set(clock_div_4);

  pinMode(2, OUTPUT);  // Si5351 off
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  
  cw_s(); cw_a(); cw_6(); cw_b(); cw_s(); cw_s(); // Set callsign of the CW id at startup

  delay(80);
  pinMode(A0, OUTPUT); // GPS VCC on
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  delay(1500);
  Serial.begin(9600);

  //delay(2500); Serial.write("$PMTK886,3*2B\r\n");   //set balloon mode for QUECTEL L70 GPS module
  delay(2500); Serial.write("$PCAS04,1*18\r\n");      //GPS only ATGM336
  //delay(2500); Serial.write("$PCAS04,2*1B\r\n");    //BD  only ATGM336
  //delay(2500); Serial.write("$PCAS04,7*1E\r\n");    //All ATGM336 
  //delay(2500); Serial.print("$PSIMNAV,W,3*3A\r\n"); //Sets Balloon mode of L70
  //delay(2500); Serial.write("$PMTK127*36\r\n");     //Erase EPO data stored in the flash memory - L70
  //delay(2500); Serial.write("$PMTK120*31\r\n");     //Erase aiding data stored in the flash memory - L70
  //delay(2500); Serial.write("$PMTK104*37\r\n");     //resets the GNSS module to the factory default - L70

  wdt_reset();
 }

ISR(TIMER1_COMPA_vect) // Interrupt to drive the TX timing
 {  proceed = true; }

void loop()
 {
  wdt_reset();
  while (Serial.available() > 0)
  if (gps.encode(Serial.read()))            // GPS related functions need to be in here to work with tinyGPS Plus library
  {
    wdt_reset();
  }
  if (gps.location.isValid())
  {
  if (timeStatus() == timeNotSet)           // Only sets time if already not done previously
  {
    setGPStime();                           // Sets system time to GPS UTC time for sync
  }
    TXtiming();                             // Process timing
  }
  }
