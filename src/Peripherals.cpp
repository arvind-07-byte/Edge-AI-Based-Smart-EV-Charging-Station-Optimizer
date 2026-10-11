#include <Arduino.h>
#include <DHT.h>
#include "State.h"
#include "Peripherals.h"
#include "config.h"


DHT   dht(DHT_PIN , DHT_TYPE);


float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void sample_sensors(void)
{
    int raw_current = analogRead(CURRENT_PIN); // 0 to 4095  (0 to 32A)
    int raw_voltage = analogRead(VOLTAGE_PIN); // 0 to 4095  (0 to 250V)
   power = voltage * current; // in watts
    voltage = mapFloat(raw_voltage, 0, 4095, 0, 250); // map to 0-250V
   if(bayStatus == "CHARGING")
   {
    current = mapFloat(raw_current, 0, 4095, 0, 32);} // map to 0-32A
    //to read temperature 
  float t = dht.readTemperature(DHT_PIN );
  if (!(isnan(t))) temperature = t;
    
   }
float recentAvgCurrent(void)
{float sum =0;
  for (int i =0;i<5;i++)
  {
    sum += current;
  }
  return sum / 5;
}
// Store previous button states to detect state transitions (button press edge)
bool lastPluginState = HIGH;
bool lastPlugoutState = HIGH;

void plug_status(void)
{
  bool currentPluginReading = digitalRead(BTN_PLUGIN);
  bool currentPlugoutReading = digitalRead(BTN_PLUGOUT);

  // Detect Plug-In Press (Transition from HIGH to LOW)
  if (currentPluginReading == LOW && lastPluginState == HIGH)
  {  sessionStartMs = millis(); // Record the start time of the charging session
    if (bayStatus == "FREE")
    {
      bayStatus = "CHARGING";
      Serial.println("Bay 1 plugin detected , and CHARGING");
      // turn on relay to start charging
      digitalWrite(RELAY_PIN, HIGH); // Activate relay to start charging 
   }
  }

  // Detect Plug-Out Press (Transition from HIGH to LOW)
  if (currentPlugoutReading == LOW && lastPlugoutState == HIGH)
  {
    if (bayStatus == "CHARGING")
    {
      bayStatus = "FREE";
      Serial.println("Bay 1 plugout detected , and FREE");
    }
  }

  // Save current states for the next iteration cycle
  lastPluginState = currentPluginReading;
  lastPlugoutState = currentPlugoutReading;
}


/*void update_led_status(void){

   //  if bay_status is charging , turn on green led
   
   if(bayStatus == "FREE")
     {

      digitalWrite(LED_GREEN, HIGH); 
      digitalWrite(LED_YELLOW, LOW); 
   }

   else{
      
      digitalWrite(LED_GREEN, LOW); 
      digitalWrite(LED_YELLOW, HIGH); 
   }
   
}*/