void TXtiming() // Timing
{
  static int8_t lastHandledMinute = -1;   // minute we've already acted on

  if ((timeStatus() == timeSet) && (second() <= 1) && (minute() != lastHandledMinute))
  {
    lastHandledMinute = minute();
    setGPStime();

    if ((minute() % 2) == 0)              // any even minute = start of a WSPR slot
    {
      if (telemetry_set == 0)             // prepare data for the pair
      {
        loc4calc();
        call_telem();
        dbm_alt();
        loc_dbm_telem();
        telemetry_set = 1;
        geofence_check();
        freq = (Outside == true) ? WSPR_FREQ : ISM_WSPR_FREQ;
      }

      if (telemetry_set == 1)             // standard WSPR message, first slot
      {
        GPS_VCC_off();
        rf_on();
        setModeWSPR();
        encode();
        telemetry_set = 2;
      }
      else if (telemetry_set == 2)        // telemetry message, next slot
      {
        setModeWSPR_telem();
        encode();
        rf_off();
        GPS_VCC_on();
        telemetry_set = 0;
        if (w < 4) w++;
        if (w == 4) software_reset();
      }
    }
  }
}