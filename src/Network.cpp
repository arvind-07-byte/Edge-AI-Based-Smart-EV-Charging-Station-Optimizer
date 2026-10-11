
#include <WiFi.h>
#include <string.h>
#include "network.h"
#include "config.h"
#include "rpc.h"

WiFiClient espClient;
PubSubClient mqtt(espClient);

// Receives RPC messages sent by ThingsBoard.
void mqttCallback(char* topic, byte* payload,
                  unsigned int length) {
    const char* prefix = "v1/devices/me/rpc/request/";

    if (strncmp(topic, prefix, strlen(prefix)) != 0) {
        return;
    }

    String requestId =
        String(topic).substring(strlen(prefix));

    char message[256];

    if (length >= sizeof(message)) {
        Serial.println("RPC payload too large");
        return;
    }

    memcpy(message, payload, length);
    message[length] = '\0';

    Serial.print("RPC received: ");
    Serial.println(message);

    handleRpc(requestId, message);
}

void connectWiFi() {
    Serial.print("Connecting to WiFi");

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED &&
           millis() - start < 15000) {
        delay(300);
        Serial.print(".");
    }

    Serial.println(
        WiFi.status() == WL_CONNECTED
        ? " connected."
        : " FAILED (will retry)."
    );
}

void connectMQTT() {
    if (WiFi.status() != WL_CONNECTED) {
        return;
    }

    Serial.print("Connecting to ThingsBoard MQTT...");

    if (mqtt.connect(BAY_ID, TB_TOKEN, NULL)) {
        Serial.println(" connected.");

        mqtt.setCallback(mqttCallback);

        if (mqtt.subscribe("v1/devices/me/rpc/request/+")) {
            Serial.println("RPC subscription successful.");
        } else {
            Serial.println("RPC subscription failed.");
        }
    } else {
        Serial.print(" failed, rc=");
        Serial.println(mqtt.state());
        delay(1000);
    }
}
