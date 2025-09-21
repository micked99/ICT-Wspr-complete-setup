/*
   Telemetry functions
*/

void setGPStime() // Sets the system time from the GPS
 {
  if (gps.time.isValid())
 {
    Hour = gps.time.hour();
    Minute = gps.time.minute();
    Second = gps.time.second();
    setTime(Hour, Minute, Second, 1, 1, 2000); // (hr,min,sec,day,month,yr)
 }
 }

void locjt9() // Calculates full locator from GPS
 {
  lon = (gps.location.lng() * 100000) + 18000000L;
  lat = (gps.location.lat() * 100000) +  9000000L;
  char MH[13] = {'A', 'A', 'A', 'A', 'A', 'A', '-', 'A', 'A', 'A', 'A', 'A', 'A'};
  MH[5] += lat % 25;
  unsigned long q = lat / 25;
  MH[4] += q % 25;
  q = q / 25;
  MH[3] += q % 25;
  q = q / 25;
  MH[2] += q % 25;
  q = q / 25;
  MH[1] += q % 25;
  MH[0] += q / 25;
  MH[12] += lon % 25;
  q = lon / 25;
  MH[11] += q % 25;
  q = q / 25;
  MH[10] += q % 25;
  q = q / 25;
  MH[9] += q % 25;
  q = q / 25;
  MH[8] += q % 25;
  MH[7] += q / 25;
  String locjt9_txt = "";
  int i = 0;
  while (i < 13) {
  locjt9_txt += MH[i];
  i++;
 }
  locjt9_txt.toCharArray(message_jt9, 14);
 }

void loc4calc() // Determine the locator from the GPS data
 {
  lon = (gps.location.lng() * 100000) + 18000000L;
  lat = (gps.location.lat() * 100000) +  9000000L;
  char MH[4] = {'A', 'A', '0', '0'};
  MH[0] +=  lon / 2000000;
  MH[1] +=  lat / 1000000;
  MH[2] += (lon % 2000000) / 200000;
  MH[3] += (lat % 1000000) / 100000;
  String loc4_txt = "";
  int i = 0;
  while (i < 4) {
    loc4_txt += MH[i];
    i++;
 }
  loc4_txt.toCharArray(loc4, 5);
 }

void call_telem() // Determine the telemetry callsign
 {
  alt_meters = gps.altitude.meters();
  char MH[2] = {'A', 'A'};
  MH[0] += ((lon % 200000) / 8333);
  MH[1] += ((lat % 100000) / 4166);
  
  // Telemetry channel 0-19 - 00 01 02 - 08 09 - Q0 Q1 Q2 - Q8 Q9
  call_telemetry[0] = '0';  // set 0 or Q
  call_telemetry[2] = '8';  // set 0 to 9
    
  int a = MH[0] - 'A';
  int b = MH[1] - 'A';
  int c = a * 24 + b;
  int d = int(alt_meters / 20);
  long e = (long)(1068L * c + d);
  long f = float(e / 17576L);
  if (f < 10)
  call_telemetry[1] = f + '0';
  else
  call_telemetry[1] = f - 10 + 'A';
  long g = e - f * 17576L;
  int h = int(g / 676);
  call_telemetry[3] = h + 'A';
  long i = g - h * 676L;
  int j = int(i / 26);
  call_telemetry[4] = j + 'A';
  long k = i - j * 26L;
  int l = int(k / 1);
  call_telemetry[5] = l + 'A';
 }

void loc_dbm_telem() // Determine the locator and dBm value for the telemetry transmission
 {
  Sats =  gps.satellites.value();
  gps_speed = gps.speed.knots();
  int wADC = 0;
  int temp = 0;
  float volt = 0;
  int sensorVolt = 0;
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);
  delay(20);
  for (int i=0;i<5;i++)
 {
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA, ADSC));
  wADC = wADC + ADCW;
  delay(20);
 }
  wADC = wADC / 5;
  temp = (wADC - 305.11 ) / 1.15;
  delay(20);
  analogReference(INTERNAL);
  for (int i=0;i<5;i++)
 {
  sensorVolt = sensorVolt + analogRead(0); //analogRead(0) for the old boards
 }
  sensorVolt = sensorVolt / 5;   // mark out for fixed voltage
  volt = sensorVolt * 1.1f;      // mark out for fixed voltage
  volt = volt / 1023.0f;         // mark out for fixed voltage
  volt = volt * 4.18f;           // mark out for fixed voltage
  // volt = 3.94;                // --Remove if sensor voltage is used--
  if (volt < 3.0) volt = 3.0;    // mark out for fixed voltage
  if (volt > 4.95) volt = 4.95;  // mark out for fixed voltage
  if (temp < -49) temp = -49;
  if (temp > 39) temp = 39;
  int GPS = 0;
  if ((lon != oldlon) || (lat != oldlat))
  GPS = 1;
  else
  GPS = 0;
  oldlon = lon;
  oldlat = lat;
  if (Sats < 5) Sats = 0; else Sats = 1;
  int temp_raw = (int)(1024L * (temp * 0.01 + 2.73)) / 5;
  temp_raw = (int)(temp_raw - 457) / 2;
  float batt_raw = (int)(1024L * volt) / 5;
  batt_raw = (batt_raw - 614) / 10;
  float round_number = round(batt_raw);
  if (round_number > batt_raw) round_number = round_number - 1;
  batt_raw = (int)round_number;
  long x = (temp_raw * 40L ) + batt_raw;
  long y = (x * 42L) + (int)gps_speed / 2;
  long z = (y * 2L) + (int)GPS;
  long xy = (z * 2L) + (int)Sats;
  long a = (int)(xy / 34200L);
  loc_telemetry[0] = a + 'A';
  long b = xy - (34200L * a);
  long c = (int)(b / 1900L);
  loc_telemetry[1] = c + 'A';
  long d = b - (1900L * c);
  long e = (int)(d / 190L);
  loc_telemetry[2] = e + '0';
  long f = d - (190L * e);
  long g = (int) (f / 19L);
  loc_telemetry[3] = g + '0';
  dbm_telemetry = f - (g * 19L);
  if (dbm_telemetry == 0) dbm_telemetry = 0;
  else if (dbm_telemetry == 1) dbm_telemetry = 3;
  else if (dbm_telemetry == 2) dbm_telemetry = 7;
  else if (dbm_telemetry == 3) dbm_telemetry = 10;
  else if (dbm_telemetry == 4) dbm_telemetry = 13;
  else if (dbm_telemetry == 5) dbm_telemetry = 17;
  else if (dbm_telemetry == 6) dbm_telemetry = 20;
  else if (dbm_telemetry == 7) dbm_telemetry = 23;
  else if (dbm_telemetry == 8) dbm_telemetry = 27;
  else if (dbm_telemetry == 9) dbm_telemetry = 30;
  else if (dbm_telemetry == 10) dbm_telemetry = 33;
  else if (dbm_telemetry == 11) dbm_telemetry = 37;
  else if (dbm_telemetry == 12) dbm_telemetry = 40;
  else if (dbm_telemetry == 13) dbm_telemetry = 43;
  else if (dbm_telemetry == 14) dbm_telemetry = 47;
  else if (dbm_telemetry == 15) dbm_telemetry = 50;
  else if (dbm_telemetry == 16) dbm_telemetry = 53;
  else if (dbm_telemetry == 17) dbm_telemetry = 57;
  else if (dbm_telemetry == 18) dbm_telemetry = 60;
 }
 
#define WSPR_TONE_SPACING  146     // ~1.46 Hz
#define WSPR_DELAY         683     // Delay value for WSPR

 void geofence_check()
 {
  Outside = true;
  for (unsigned int GridLoop = 0; GridLoop < sizeof(NoTXGrids); GridLoop = GridLoop + 5) //Iterate between Geo-Fenced grids
 {
  for (int CharLoop = 0; CharLoop < 4; CharLoop++)
 {
      TestGrid[CharLoop] = pgm_read_byte_near (NoTXGrids + CharLoop + GridLoop);  //Copy a Grid string from program memory to RAM variable.
 }
  if ((loc4[0] == TestGrid[0]) && (loc4[1] == TestGrid[1]) && (loc4[2] == TestGrid[2]) && (loc4[3] == TestGrid[3])) {
      Outside = false; //We found a match between the current location and a Geo-Fenced Grid
      break;
 }
 }
 }

 void dbm_alt() // Coarse altitude for WSPR standard message dBm value
 {
/*  
alt_meters = gps.altitude.meters();
if (alt_meters >= 15000) dbm = 60;
else if (alt_meters >= 14600) dbm = 57;
else if (alt_meters >= 14200) dbm = 53;
else if (alt_meters >= 13800) dbm = 50;
else if (alt_meters >= 13400) dbm = 47;
else if (alt_meters >= 13000) dbm = 43;
else if (alt_meters >= 12600) dbm = 40;
else if (alt_meters >= 12200) dbm = 37;
else if (alt_meters >= 11800) dbm = 33;
else if (alt_meters >= 11400) dbm = 30;
else if (alt_meters >= 11000) dbm = 27;
else if (alt_meters >= 10600) dbm = 23;
else if (alt_meters >= 10200) dbm = 20;
else if (alt_meters >= 9800) dbm = 17;
else if (alt_meters >= 8000) dbm = 13;
else if (alt_meters >= 6000) dbm = 10;
else if (alt_meters >= 3000) dbm = 7;
else if (alt_meters >= 1000) dbm = 3;
else dbm = 0;  
*/ 

alt_meters = gps.altitude.meters();
if (alt_meters >= 15000) dbm = 0;
else if (alt_meters >= 14600) dbm = 3;
else if (alt_meters >= 14200) dbm = 7;
else if (alt_meters >= 13800) dbm = 10;
else if (alt_meters >= 13400) dbm = 13;
else if (alt_meters >= 13000) dbm = 17;
else if (alt_meters >= 12600) dbm = 20;
else if (alt_meters >= 12200) dbm = 23;
else if (alt_meters >= 11800) dbm = 27;
else if (alt_meters >= 11400) dbm = 30;
else if (alt_meters >= 11000) dbm = 33;
else if (alt_meters >= 10600) dbm = 37;
else if (alt_meters >= 10200) dbm = 40;
else if (alt_meters >= 9800) dbm = 43;
else if (alt_meters >= 8000) dbm = 47;
else if (alt_meters >= 6000) dbm = 50;
else if (alt_meters >= 3000) dbm = 53;
else if (alt_meters >= 1000) dbm = 57;
else dbm = 60;  

 }
 
void setModeWSPR()
 {
  symbol_count = WSPR_SYMBOL_COUNT;
  tone_spacing = WSPR_TONE_SPACING;
  tone_delay = WSPR_DELAY;
  CTC = WSPR_CTC;
  memset(tx_buffer, 0, 255); // Clears Tx buffer from previous operation.
  jtencode.wspr_encode(call, loc4, dbm, tx_buffer);
 }

void setModeWSPR_telem()
 {
  symbol_count = WSPR_SYMBOL_COUNT;
  tone_spacing = WSPR_TONE_SPACING;
  tone_delay = WSPR_DELAY;
  CTC = WSPR_CTC;
  memset(tx_buffer, 0, 255); // Clears Tx buffer from previous operation.
  jtencode.wspr_encode(call_telemetry, loc_telemetry, dbm_telemetry, tx_buffer);
 }

void Si5351a_Write_Reg (byte regist, byte value){   // Writes "byte" into "regist" of Si5351a via I2C  
  Wire.beginTransmission(96);    //111 or 96        // Starts transmission as master to slave 96, which is the 
                                                    // I2C address of the Si5351a (see Si5351a datasheet)
  Wire.write(regist);                               // Writes a byte containing the number of the register
  Wire.write(value);                                // Writes a byte containing the value to be written in the register
  Wire.endTransmission();                           // Sends the data and ends the transmission
 }

void SetFrequency (uint32_t frequency) { // Frequency in Hz; must be within [7810 Hz to 200 Mhz]
  #define F_XTAL 26000000;               // Frequency of Quartz-Oszillator 26000000 of the 5351
  #define c 924000;                      // "c" part of Feedback-Multiplier from XTAL to PLL
  uint64_t fvco;                         // VCO frequency (600-900 MHz) of PLL
  uint32_t outdivider;                   // Output divider in range [4,6,8-900], even numbers preferred
  byte R = 1;                            // Additional Output Divider in range [1,2,4,...128]
  byte a;                                // "a" part of Feedback-Multiplier from XTAL to PLL in range [15,90]
  unsigned long b;                       // "b" part of Feedback-Multiplier from XTAL to PLL
  float f;                               // floating variable, needed in calculation
  unsigned long MS0_P1;                  // Si5351a Output Divider register MS0_P1, P2 and P3 are hardcoded below
  unsigned long MSNA_P1;                 // Si5351a Feedback Multisynth register MSNA_P1
  unsigned long MSNA_P2;                 // Si5351a Feedback Multisynth register MSNA_P2
  unsigned long MSNA_P3;                 // Si5351a Feedback Multisynth register MSNA_P3
 //outdivider 77000000000 or 30000000000 // If bad decodes at 24/28Mhz try the other value
  outdivider = 30000000000 / frequency;  // With 900 MHz beeing the maximum internal PLL-Frequency
  while (outdivider > 900){              // If output divider out of range (>900) use additional Output divider
  R = R * 2;
  outdivider = outdivider / 2;
 }
  if (outdivider % 2) outdivider--;    // finds the even divider which delivers the intended Frequency
  fvco = frequency;
  fvco = fvco * outdivider * R /100 ;  // Calculate the PLL-Frequency (given the even divider)
  switch (R){                          // Convert the Output Divider to the bit-setting required in register 44
  case 1: R = 0; break;                // Bits [6:4] = 000
  case 2: R = 16; break;               // Bits [6:4] = 001
  case 4: R = 32; break;               // Bits [6:4] = 010
  case 8: R = 48; break;               // Bits [6:4] = 011
  case 16: R = 64; break;              // Bits [6:4] = 100
  case 32: R = 80; break;              // Bits [6:4] = 101
  case 64: R = 96; break;              // Bits [6:4] = 110
  case 128: R = 112; break;            // Bits [6:4] = 111
  }
  a = fvco / F_XTAL;                   // Multiplier to get from Quartz-Oscillator Freq. to PLL-Freq.
  f = fvco - a * F_XTAL;               // Multiplier = a+b/c
  f = f * c;                           // this is just "int" and "float" mathematics
  f = f / F_XTAL;
  b = f;
  MS0_P1 = 128 * outdivider - 512;     // Calculation of Output Divider registers MS0_P1 to MS0_P3
                                       // MS0_P2 = 0 and MS0_P3 = 1; these values are hardcoded, see below
  f = 128 * b / c;                     // Calculation of Feedback Multisynth registers MSNA_P1 to MSNA_P3
  MSNA_P1 = 128 * a + f - 512;
  MSNA_P2 = f;
  MSNA_P2 = 128 * b - MSNA_P2 * c; 
  MSNA_P3 = c;
  
  //Si5351a_Write_Reg (16, 128);// Disable output during the following register settings 
  Si5351a_Write_Reg (26, (MSNA_P3 & 65280) >> 8);   // Bits [15:8] of MSNA_P3 in register 26
  Si5351a_Write_Reg (27, MSNA_P3 & 255);            // Bits [7:0]  of MSNA_P3 in register 27
  Si5351a_Write_Reg (28, (MSNA_P1 & 196608) >> 16); // Bits [17:16] of MSNA_P1 in bits [1:0] of register 28
  Si5351a_Write_Reg (29, (MSNA_P1 & 65280) >> 8);   // Bits [15:8]  of MSNA_P1 in register 29
  Si5351a_Write_Reg (30, MSNA_P1 & 255);            // Bits [7:0]  of MSNA_P1 in register 30
  Si5351a_Write_Reg (31, ((MSNA_P3 & 983040) >> 12) | ((MSNA_P2 & 983040) >> 16)); // Parts of MSNA_P3 und MSNA_P1
  Si5351a_Write_Reg (32, (MSNA_P2 & 65280) >> 8);   // Bits [15:8]  of MSNA_P2 in register 32
  Si5351a_Write_Reg (33, MSNA_P2 & 255);            // Bits [7:0]  of MSNA_P2 in register 33
  Si5351a_Write_Reg (42, 0);                        // Bits [15:8] of MS0_P3 (always 0) in register 42
  Si5351a_Write_Reg (43, 1);                        // Bits [7:0]  of MS0_P3 (always 1) in register 43
  Si5351a_Write_Reg (44, ((MS0_P1 & 196608) >> 16) | R);  // Bits [17:16] of MS0_P1 in bits [1:0] and R in [7:4]
  Si5351a_Write_Reg (45, (MS0_P1 & 65280) >> 8);    // Bits [15:8]  of MS0_P1 in register 45
  Si5351a_Write_Reg (46, MS0_P1 & 255);             // Bits [7:0]  of MS0_P1 in register 46
  Si5351a_Write_Reg (47, 0);                        // Bits [19:16] of MS0_P2 and MS0_P3 are always 0
  Si5351a_Write_Reg (48, 0);                        // Bits [15:8]  of MS0_P2 are always 0
  Si5351a_Write_Reg (49, 0);                        // Bits [7:0]   of MS0_P2 are always 0
  if (outdivider == 4){
  Si5351a_Write_Reg (44, 12 | R);                   // Special settings for R = 4 (see datasheet)
  Si5351a_Write_Reg (45, 0);                        // Bits [15:8]  of MS0_P1 must be 0
  Si5351a_Write_Reg (46, 0);                        // Bits [7:0]  of MS0_P1 must be 0
 } 
  //Si5351a_Write_Reg (177, 32);                    // This resets PLL A
 }

void encode() // Loop through the string, transmitting one character at a time
 {
  // Set up Timer1 for interrupts every symbol period.
  cli(); //Disable global interrupts
  TCCR1A = 0x00;                  // Set entire TCCR1A register to 0; disconnects
                                  // interrupt output pins, sets normal waveform
                                  // mode.  We're just using Timer1 as a counter.
  TCNT1  = 0x00;                  // Initialize counter value to 0.
  TCCR1B = (1 << CS12) |          // Set CS12 and CS10 bit to set prescale
           (1 << CS10) |          // to /1024
           (1 << WGM12);          // turn on CTC
                                  // which gives, 64 us ticks
  TIMSK1 = (1 << OCIE1A);         // Enable timer compare interrupt.
  OCR1A = CTC;                    // Set up interrupt trigger count;
  sei(); //Enable global interrupts
  uint8_t i;
  Si5351a_Write_Reg (17, 128);         // Disable output CLK1
  Si5351a_Write_Reg (18, 128);         // Disable output CLK2
  Si5351a_Write_Reg (16, pwr_level);   // Enable output CLK0, set crystal as source and Integer Mode on PLLA
  for (i = 0; i < symbol_count; i++)   // Now transmit the channel symbols
 {
  SetFrequency((freq * 100) + (tx_buffer[i] * tone_spacing));
  //Si5351a_Write_Reg (16, 79);        //Get rid of ticking
  proceed = false; // TX Timing interrupt
  while (!proceed); // TX Timing interrupt
  wdt_reset();
 }
  Si5351a_Write_Reg (16, 128);         // Disable output CLK0
  // Disable Timer1
  cli(); //Disable global interrupts
  TCCR1B = 0x00;
  sei(); //Enable global interrupts
 }

 void software_reset()
{
  wdt_reset(); 

  exit (1);  // loop forever
}  

void rf_on() // Turn on the high-side switch, activating the transmitter
 {
  delay(10);
  digitalWrite(2, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
 }
 
void rf_off() // Turn off the high-side switch
 {
  digitalWrite(2, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
 }

void GPS_VCC_on()
 {
  Serial.begin(9600);
  delay(10);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
  digitalWrite(A7, HIGH);
 }

void GPS_VCC_off()
 {
  Serial.end();
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  digitalWrite(A7, LOW);
 }
  

