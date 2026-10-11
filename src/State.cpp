#include "State.h"


String bayStatus = "FREE";
float voltage = 0.0, current = 0.0, power = 0.0, energyWh = 0.0, temperature = 0.0;


unsigned long sessionStartMs = 0;

// edge ai varilables
float predictedArrivalProb = 0.0f;
int predictedDurationMin = 0;
int lastHourOfDay = 12;

// optimization variables
String loadDecision = "ALLOW";
int throttleLevel = 100; // percentage of full power to allow (0-100)
bool overloadActive = false;
float predictionThreshold = 0.5; // threshold for predicted arrival probability to trigger optimization   
int peakTariffStartHr = 17; // 5 PM
int peakTariffEndHr = 21; // 9 PM
int overloadCurrentA = 16; // maximum current in Amps before throttling 
int maxStationLoadW = 3000; // maximum station load in watts before throttling
bool manualOverrideActive = false;