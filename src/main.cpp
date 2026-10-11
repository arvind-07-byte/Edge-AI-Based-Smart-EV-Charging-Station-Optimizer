#include <Arduino.h>
#include <WiFi.h>
#include "State.h"
#include "config.h"
#include "Peripherals.h"
#include "Network.h"
#include "Telemetry.h"
#include "model.h"
#include "edge_ai.h"
#include "optimization.h"
void sample_sensors();

void setup()
{

    Serial.begin(115200);
    dht.begin();  // initialise sesnor


    //config esp32 with real time clock
    configTime(0,0,"pool.ntp.org","time.nist.gov");
pinMode(BTN_PLUGIN,INPUT_PULLUP); // set the pin as input with pullup resistor
pinMode(BTN_PLUGOUT,INPUT_PULLUP); // set the pin as input with pullup resistor
pinMode(RELAY_PIN, OUTPUT); // set the pin as output for relay control
pinMode(LED_GREEN, OUTPUT); // set the pin as output for green led
pinMode(LED_RED, OUTPUT); // set the pin as output for red led
pinMode(LED_YELLOW, OUTPUT); // set the pin as output for yellow led

// connect to WiFi and MQTT

connectWiFi();

// configure server then connect to MQTT
mqtt.setServer(MQTT_SERVER, MQTT_PORT);
connectMQTT();
}

unsigned long now;
unsigned long last_print;

void loop()
{ mqtt.loop(); // process incoming MQTT messages
    //print vals every 5 sec
    now = millis();
    if((now - last_print) > 5000)
    {
        last_print = now;
        sample_sensors(); // sample the sensors and update the global variables
 
// run edge ai inference to predict arrival probability and duration
runEdgeAIInference();
// decide load based on predictions 
runOptimization();

// publish telemetry to MQTT
publishTelemetry();

    }
applyRelayDutyCycle(); // apply the throttleLevel to the relay via duty-cycling
plug_status(); // check the plug in and plug out status
updateLeds(); // update the LED status based on the bay status
}

