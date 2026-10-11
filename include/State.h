#ifndef STATE_H
#define STATE_H

#include <Arduino.h>
// ---------------------------------------------------------------------
// Live bay state
// ---------------------------------------------------------------------
extern String bayStatus;
extern float voltage, current, power, energyWh, temperature;

// ---------------------------------------------------------------------
// Timing / debounce bookkeeping
// ---------------------------------------------------------------------


extern unsigned long sessionStartMs;
// ---------------------------------------------------------------------
// Edge AI variables
// ---------------------------------------------------------------------

extern float predictedArrivalProb;
extern int predictedDurationMin;
extern int lastHourOfDay;

extern bool manualOverrideActive;
extern String loadDecision;
extern int throttleLevel; // percentage of full power to allow (0-100)
extern bool overloadActive;
extern float predictionThreshold; // threshold for predicted arrival probability to trigger optimization   
extern int peakTariffStartHr; // 5 PM
extern int peakTariffEndHr ; // 9 PM
extern int overloadCurrentA; // maximum current in Amps before throttling
extern int maxStationLoadW; // maximum station load in watts before throttling
#endif