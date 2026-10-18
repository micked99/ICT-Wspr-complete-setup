const uint8_t TLM_SLOT  = 2;   // slot of the 10-min cycle that carries the telemetry message: 0, 2, 4, 6 or 8
const uint8_t LAST_SLOT = 6;   // last TX slot in the cycle: 6 = four slots (0,2,4,6), 8 = all five

void TXtiming() // Timing
{
  static int8_t lastHandledMinute = -1;   // minute we've already acted on
  static bool firstTx = true;             // true until the first transmission after power-up

  if ((timeStatus() == timeSet) && (second() <= 1) && (minute() != lastHandledMinute))
  {
    lastHandledMinute = minute();
    setGPStime();

    uint8_t slot = minute() % 10;         // position in the 10-minute cycle

    if (((slot % 2) == 0) && (slot <= LAST_SLOT))
    {
      if ((slot == 0) || (telemetry_set == 0))   // fresh data at slot 0, or at first slot after power-up
      {
        loc4calc();
        call_telem();
        dbm_alt();
        loc_dbm_telem();
        geofence_check();
        freq = (Outside == true) ? WSPR_FREQ : ISM_WSPR_FREQ;
        telemetry_set = 1;                // data is ready
      }

      GPS_VCC_off();
      rf_on();
      if ((slot == TLM_SLOT) && !firstTx) setModeWSPR_telem();   // telemetry slot, but never as the very first TX
      else                                setModeWSPR();         // all other slots = standard message
      encode();
      rf_off();
      GPS_VCC_on();
      firstTx = false;

      if (slot == LAST_SLOT) telemetry_set = 0;   // cycle finished, re-prepare at next slot 0
    }
  }
}