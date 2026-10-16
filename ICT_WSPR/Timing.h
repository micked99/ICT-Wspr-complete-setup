void TXtiming() // Timing
   {
    // run additional scripts here to generate data prior to TX if there is a large delay involved.

    static uint8_t lastHandledSecond = 255;   // last second() value we already acted on

    // Fires once per second, with a 2-second catch window (second 0 or 1) instead of
    // requiring second()==0 exactly. If loop() is ever delayed past second 0, this
    // still catches the slot on second 1 instead of skipping the whole 2-minute window.
    if ((timeStatus() == timeSet) && (second() <= 1) && (second() != lastHandledSecond))
   {
    lastHandledSecond = second();
    setGPStime();

    uint8_t slot = minute() % 10;   // which 2-minute slot of the 10-minute cycle we're in

   if ((slot == 0) && (telemetry_set == 0))
   {
      loc4calc();             // Get position
      call_telem();           // Update WSPR telemetry callsign based on previous information : position and altitude in meters
      dbm_alt();              // Calculate the dBm field to encode coarse altitude
      loc_dbm_telem();        // Update WSPR telemetry locator and dbm. Get temperature, voltage, speed in knots, GPS status and sats number
      telemetry_set = 1;      // Set WSPR as next in sequence
      geofence_check();

   if (Outside == true)
      freq = WSPR_FREQ;       // Set WSPR frequency
   else   
      freq = ISM_WSPR_FREQ;   // Set ISM WSPR frequency
   }
        
    if ((slot == 0) && (telemetry_set == 1)) // TX WSPR telemetry message
   {
      GPS_VCC_off();
      rf_on();
      setModeWSPR();         // Set WSPR standard or WSPR-telem mode
      encode();              // Begin radio transmission
      rf_off();
      GPS_VCC_on();
      telemetry_set = 2;     // Set WSPR as next in sequence
   }
    
    else if ((slot == 2) && (telemetry_set != 0))    // TX WSPR standard message
   {
      GPS_VCC_off();
      rf_on();
      setModeWSPR_telem();    // Set WSPR standard or WSPR-telem 
      encode();               // Begin radio transmission
      rf_off();
      GPS_VCC_on();
      telemetry_set = 3;      // Set WSPR as next in sequence
    }
    
    else if ((slot == 4) && (telemetry_set != 0))    // TX WSPR standard message
    {
      GPS_VCC_off(); 
      rf_on();
      setModeWSPR();          // Set WSPR standard or WSPR-telem 
      encode();               // Begin radio transmission
      rf_off(); 
      GPS_VCC_on();
      telemetry_set = 4;      // Set WSPR as next in sequence
   }
   
    else if ((slot == 6) && (telemetry_set != 0))    // TX WSPR standard message
   {
      GPS_VCC_off();
      rf_on();
      setModeWSPR();                  // Set WSPR standard or WSPR-telem 
      encode();                       // Begin radio transmission
      rf_off();
      GPS_VCC_on();
      telemetry_set = 0;              // Set WSPR as next in sequence
   } 
   }
   }
