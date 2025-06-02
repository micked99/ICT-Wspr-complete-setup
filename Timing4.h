void TXtiming() // Timing
   {
    // run additional scripts here to generate data prior to TX if there is a large delay involved.
    if ((timeStatus() == timeSet) && (second() == 0) && (minute() >= 0)) 
   {
    setGPStime();

   if ((minute() % 02 == 0) && (second() <= 0) && (telemetry_set == 0))
   {
      loc4calc();                  // Get position
      call_telem();                // Update WSPR telemetry callsign based on previous information : position and altitude in meters
      dbm_alt();                   // Calculate the dBm field to encode coarse altitude
      loc_dbm_telem();             // Update WSPR telemetry locator and dbm. Get temperature, voltage, speed in knots, GPS status and sats number
      telemetry_set = 1;           // Set WSPR as next in sequence
      geofence_check();

    if (Outside == true)
      freq = WSPR_FREQ;             // Set WSPR frequency
    else   
      freq = ISM_WSPR_FREQ;         // Set ISM WSPR frequency
    }
        
    if ((minute() % 02 == 0) && (second() <= 0) && (telemetry_set == 1)) // TX WSPR standard message
    {
      //freq = freq + (random(-4, 5) * 2 * 1.46); // Randomize WSPR frequency
      GPS_VCC_off();
      rf_on();
      setModeWSPR();                // set WSPR standard mode
      encode();                     // begin radio transmission
      //rf_off();
      //GPS_VCC_on();
      telemetry_set = 2;            // Set WSPR as next in sequence
      if (w < 4) w++;
      if (w == 4) software_reset ();
    }
    
    else if ((minute() % 02 == 0) && (second() <= 0) && (telemetry_set == 2))    // TX WSPR telemetry message
    {
      //freq = freq + (random(-4, 5) * 2 * 1.46); // Randomize WSPR frequency
      //GPS_VCC_off();
      //rf_on();
      setModeWSPR_telem();          // set WSPR telemetry mode
      encode();                     // begin radio transmission
      rf_off();
      GPS_VCC_on();
      telemetry_set = 0;            // Set WSPR as next in sequence
    }
    } 
    }
