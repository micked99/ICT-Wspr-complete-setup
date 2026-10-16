/*
   Telemetry functions
*/

void setGPStime() // Sets the system time from the GPS
 {
  if (gps.time.isValid())
 {
    setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), 1, 1, 2000); // (hr,min,sec,day,month,yr)
 }
 }

void loc4calc() // Determine the locator from the GPS data
 {
  lon = (gps.location.lng() * 100000) + 18000000L;
  lat = (gps.location.lat() * 100000) +  9000000L;
  loc4[0] = 'A' + lon / 2000000;
  loc4[1] = 'A' + lat / 1000000;
  loc4[2] = '0' + (lon % 2000000) / 200000;
  loc4[3] = '0' + (lat % 1000000) / 100000;
  loc4[4] = '\0';
 }

void call_telem() // Determine the telemetry callsign
 {
  alt_meters = gps.altitude.meters();
  char MH[2] = {'A', 'A'};
  MH[0] += ((lon % 200000) / 8333);
  MH[1] += ((lat % 100000) / 4166);
  call_telemetry[0] = tlm_channel1;
  call_telemetry[2] = tlm_channel2;
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
 // analogReference(INTERNAL);     // mark out for fixed voltage
  for (int i=0;i<5;i++)
 {
 // sensorVolt = sensorVolt + analogRead(0); //analogRead(0) for the old boards // mark out for fixed voltage
 }
 // sensorVolt = sensorVolt / 5;   // mark out for fixed voltage
 // volt = sensorVolt * 1.1f;      // mark out for fixed voltage
 // volt = volt / 1023.0f;         // mark out for fixed voltage
 // volt = volt * 4.18f;           // mark out for fixed voltage
  volt = 3.51;                     // mark out for sensor voltage 
  if (volt < 3.0) volt = 3.0;      // mark out for fixed voltage
  if (volt > 4.95) volt = 4.95;    // mark out for fixed voltage
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

void geofence_check()
 {
  Outside = true;
  for (unsigned int GridLoop = 0; GridLoop < sizeof(NoTXGrids); GridLoop = GridLoop + 5)
 {
  for (int CharLoop = 0; CharLoop < 4; CharLoop++)
 {
      TestGrid[CharLoop] = pgm_read_byte_near (NoTXGrids + CharLoop + GridLoop);
 }
  if ((loc4[0] == TestGrid[0]) && (loc4[1] == TestGrid[1]) && (loc4[2] == TestGrid[2]) && (loc4[3] == TestGrid[3])) {
      Outside = false;
      break;
 }
 }
 }

void dbm_alt() // Coarse altitude for WSPR standard message dBm value
 {
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
  CTC = WSPR_CTC;
  memset(tx_buffer, 0, 255);
  jtencode.wspr_encode(call, loc4, dbm, tx_buffer);
 }

void setModeWSPR_telem()
 {
  symbol_count = WSPR_SYMBOL_COUNT;
  tone_spacing = WSPR_TONE_SPACING;
  CTC = WSPR_CTC;
  memset(tx_buffer, 0, 255);
  jtencode.wspr_encode(call_telemetry, loc_telemetry, dbm_telemetry, tx_buffer);
 }

void Si5351a_Write_Reg (byte regist, byte value) {  // Writes "byte" into "regist" of Si5351a via I2C
  Wire.beginTransmission(96);
  Wire.write(regist);
  Wire.write(value);
  Wire.endTransmission();
 }

void SetFrequency (uint32_t frequency) { // frequency is in centi-Hz (Hz x100); optimised for 14-28 MHz, no R divider

  uint64_t fvco;
  uint32_t outdivider;

  byte a;
  unsigned long b;
  float f;

  unsigned long MS0_P1;
  unsigned long MS1_P1;
  unsigned long MSNA_P1;
  unsigned long MSNA_P2;
  unsigned long MSNA_P3;

  // Target VCO ~850 MHz across 14-28 MHz band. "frequency" arrives in centi-Hz
  // (Hz x100), so the 850 MHz target is scaled to match (850,000,000 Hz x100).
  // Resulting outdivider range:
  // 14 MHz -> 60,  18 MHz -> 46,  21 MHz -> 40,  28 MHz -> 30
  // No R divider needed; outdivider always fits in phase reg (7 bits, max 127).
  outdivider = 85000000000ULL / frequency;

  // Must be even for correct 180 degree phase relationship
  if (outdivider % 2)
    outdivider--;

  // Sanity clamp — should never trigger in 14-28 MHz band
  if (outdivider < 4)   outdivider = 4;
  if (outdivider > 126) outdivider = 126;

  // VCO frequency, converted back down from centi-Hz to Hz to match tcxo/pll_c (which are in Hz).
  // This step is what preserves the ~1.4648 Hz WSPR tone spacing through to the PLL fractional
  // divider below — without it, tone steps collapse to whole Hz and WSPR decode breaks.
  fvco = (uint64_t)frequency * outdivider / 100;

  // -------------------------
  // CLK0 + CLK1 multisynth P1
  // -------------------------

  MS0_P1 = 128 * outdivider - 512;
  MS1_P1 = MS0_P1;

  // -------------------------
  // PLL (MSNA) calculation
  // -------------------------

  a = fvco / tcxo;

  f = fvco - a * tcxo;
  f = f * 924000UL;         // 924000UL;  "c" part of the XTAL->PLL feedback multiplier
  f = f / tcxo;
  b = f;

  f = 128.0 * b / 924000UL; // 924000UL; "c" part of the XTAL->PLL feedback multiplier
  MSNA_P1 = 128 * a + (unsigned long)f - 512;
  MSNA_P2 = 128 * b - (unsigned long)f * 924000UL; 
  MSNA_P3 = 924000UL; 

  // -------------------------
  // PLLA setup
  // -------------------------

  Si5351a_Write_Reg (26, (MSNA_P3 & 65280) >> 8);
  Si5351a_Write_Reg (27,  MSNA_P3 & 255);
  Si5351a_Write_Reg (28, (MSNA_P1 & 196608) >> 16);
  Si5351a_Write_Reg (29, (MSNA_P1 & 65280) >> 8);
  Si5351a_Write_Reg (30,  MSNA_P1 & 255);
  Si5351a_Write_Reg (31, ((MSNA_P3 & 983040) >> 12) | ((MSNA_P2 & 983040) >> 16));
  Si5351a_Write_Reg (32, (MSNA_P2 & 65280) >> 8);
  Si5351a_Write_Reg (33,  MSNA_P2 & 255);

  // -------------------------
  // CLK0 setup (no R divider — R bits in reg 44 left as 0)
  // -------------------------

  Si5351a_Write_Reg (16, 0x4F);  // Integer mode | PLLA | MS0, full drive (79 decimal)
  Si5351a_Write_Reg (42, 0);
  Si5351a_Write_Reg (43, 1);
  Si5351a_Write_Reg (44, (MS0_P1 & 196608) >> 16);
  Si5351a_Write_Reg (45, (MS0_P1 & 65280) >> 8);
  Si5351a_Write_Reg (46,  MS0_P1 & 255);
  Si5351a_Write_Reg (47, 0);
  Si5351a_Write_Reg (48, 0);
  Si5351a_Write_Reg (49, 0);

  // -------------------------
  // CLK1 setup — only when the 180° dipole hardware config is selected.
  // In standard/ground-wire mode CLK1 is left alone (it stays powered down,
  // as encode() disables it at the end of every transmission anyway).
  // -------------------------

  if (use180deg)
 {
    Si5351a_Write_Reg (17, 0x4F);  // Integer mode | PLLA | MS1, full drive (79 decimal)
    Si5351a_Write_Reg (50, 0);
    Si5351a_Write_Reg (51, 1);
    Si5351a_Write_Reg (52, (MS1_P1 & 196608) >> 16);
    Si5351a_Write_Reg (53, (MS1_P1 & 65280) >> 8);
    Si5351a_Write_Reg (54,  MS1_P1 & 255);
    Si5351a_Write_Reg (55, 0);
    Si5351a_Write_Reg (56, 0);
    Si5351a_Write_Reg (57, 0);

    // Phase offset for 180 degrees — MUST be written after all MS config
    //
    // Per Si5351 AN619: phase offset register value = outdivider (integer divider).
    // outdivider is 30-60 across 14-28 MHz, well within 7-bit reg limit (max 127).
    // CLK0 phase = 0 (reference), CLK1 phase = outdivider (180 degrees out).
    // PLLA reset latches the phase — required per AN619.
    Si5351a_Write_Reg (165, 0);                    // CLK0 phase offset = 0
    Si5351a_Write_Reg (166, (uint8_t)outdivider);  // CLK1 phase offset = 180 deg
 }

  // Reset PLLA to latch phase/frequency — required by Si5351 AN619
  Si5351a_Write_Reg (177, 32);
}

void encode()
{
  cli();

  TCCR1A = 0x00;
  TCNT1  = 0x00;

  TCCR1B = (1 << CS12) |
           (1 << CS10) |
           (1 << WGM12);

  TIMSK1 = (1 << OCIE1A);

  OCR1A = CTC;

  sei();

  uint8_t i;

  // Disable CLK2
  Si5351a_Write_Reg (18, 128);

  // CLK0 (and CLK1, if use180deg) enable is handled inside SetFrequency() on every call.

  for (i = 0; i < symbol_count; i++)
  {
    // freq is in Hz; tone_spacing = 146 = 1.46 Hz x100. Keeping this multiply in
    // centi-Hz all the way into SetFrequency() is what gives WSPR its correct
    // 1.4648 Hz tone step — do not collapse this to plain Hz before the call.
    SetFrequency((freq * 100UL) + (tx_buffer[i] * tone_spacing));

    proceed = false;

    while (!proceed);

    wdt_reset();
  }

  // Disable CLK0 and CLK1 outputs
  Si5351a_Write_Reg (16, 128);
  Si5351a_Write_Reg (17, 128);

  cli();

  TCCR1B = 0x00;

  sei();
}

void software_reset()
{
  wdt_reset();
  exit (1);  // loop forever
}

void rf_on() // Turn on Si5351
 {
  delay(10);
  digitalWrite(2, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
 }

void rf_off() // Turn off Si5351
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
 }

void GPS_VCC_off()
 {
  Serial.end();
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
  }
