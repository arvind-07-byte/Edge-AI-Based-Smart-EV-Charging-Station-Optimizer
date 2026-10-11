# Edge AI-Based Smart EV Charging Station Optimizer

## Abstract
With the rapid acceleration of electric vehicle (EV) adoption, charging infrastructure faces significant grid strain, uneven utilization, and peak-hour congestion[cite: 1]. This project presents an Edge AI-driven optimization system that operates at the charging bay level. By embedding lightweight machine learning models directly onto ESP32 microcontrollers, the system performs low-latency inference to predict local charging demand and autonomously execute load-balancing algorithms, significantly reducing peak-tariff electricity consumption while maintaining centralized cloud observability via MQTT and ThingsBoard[cite: 1].

## System Architecture
The architecture is structured across three distinct tiers[cite: 1]:

1. **Edge Layer (ESP32 Nodes):** A dedicated ESP32 microcontroller per charging bay handles real-time sensor data acquisition (voltage, current, temperature), executes the onboard AI inference, and manages the physical relay control based on local optimization logic[cite: 1].
2. **Communication Layer (MQTT):** Operates on MQTT (v3.1.1) to provide lightweight, bidirectional telemetry and attribute synchronization between the edge nodes and the cloud broker[cite: 1].
3. **Cloud Layer (ThingsBoard):** A centralized platform for time-series data storage, cross-node load aggregation, rule-chain execution for alarm generation, and RPC-based remote management[cite: 1].

## Core Capabilities
* **Embedded Machine Learning:** Evaluates feature vectors (time of day, current bay status, recent average current) locally to predict the probability of a new EV arrival and estimate the remaining duration of active sessions[cite: 1].
* **Distributed Load Optimization:** Calculates total station load against a predefined `maxStationLoadW` attribute. Dynamically transitions bay relays between `ALLOW` (100% capacity), `THROTTLE` (50-70% capacity), and `DEFER` (0% capacity) to strictly enforce peak-load limits and avoid high-tariff billing periods[cite: 1].
* **Remote Procedure Calls (RPC):** Enables station administrators to issue manual overrides (e.g., `setRelayState`) and dynamically push threshold configurations directly to the edge nodes[cite: 1].
* **Fault Tolerant Operation:** The edge-heavy design allows the system to continue basic load management and fail-safe operations even during intermittent cloud connectivity losses[cite: 1].

## Machine Learning Pipeline
Due to the strict memory constraints of the ESP32 framework, the AI models are trained offline and deployed as raw C/C++ arrays[cite: 1].
* **Training:** Models were trained on synthetic time-series charging data using Python's `scikit-learn`[cite: 1].
* **Models Used:** 
  * *Arrival Probability:* Logistic Regression[cite: 1].
  * *Session Duration:* Decision Tree Regressor (Max Depth $\le$ 5)[cite: 1].
* **Deployment:** Models were converted to embeddable C headers (`model.h`) utilizing the `micromlgen` and `m2cgen` libraries, eliminating the need for heavy on-device neural network runtimes (e.g., TensorFlow Lite)[cite: 1].

## Technology Stack
* **Microcontroller / Simulation:** ESP32 Dev Module (simulated via Wokwi)[cite: 1]
* **Sensors (Simulated):** ZMPT101B (Voltage), ACS712 (Current), DHT22 (Temperature)[cite: 1]
* **Firmware:** C/C++ (Arduino Framework), `PubSubClient` (MQTT), `ArduinoJson`[cite: 1]
* **IoT Platform:** ThingsBoard CE[cite: 1]
* **Data Science:** Python, `pandas`, `scikit-learn`[cite: 1]

## System Visualization & Simulation


### 1. Hardware Schematic (Wokwi Environment)
Illustrates the physical wiring of the ESP32 to the simulated sensor arrays, status indicators, and charging relays.
<img width="1588" height="867" alt="Screenshot 2026-10-11 171144" src="https://github.com/user-attachments/assets/3389140e-c18b-4103-abeb-b0869b13e817" />


### 2. ThingsBoard Telemetry Dashboard
Real-time cloud observability interface detailing per-bay power consumption, Edge AI demand forecasts, total aggregated station load, and active operational alarms.
<img width="1615" height="795" alt="Screenshot 2026-10-11 172017" src="https://github.com/user-attachments/assets/77547897-f17b-4ae8-85ca-cf163018d192" />


<img width="1608" height="816" alt="Screenshot 2026-10-11 171523" src="https://github.com/user-attachments/assets/9337c5be-d8e3-4d60-ae40-56311046b014" />


### 3. Edge Optimization Console Output
Serial monitor logs demonstrating the localized feature vector construction, inference execution, and resulting autonomous relay state transitions.
<img width="1500" height="849" alt="Screenshot 2026-10-11 171608" src="https://github.com/user-attachments/assets/76b5b714-ade9-4edf-bb3e-7d62a32a1df5" />


## Deployment Instructions
1. Clone this repository and open the workspace in Visual Studio Code (PlatformIO recommended).
2. Provision a new device in ThingsBoard and copy the generated device access token.
3. Update the `TB_TOKEN` constant within the firmware source code[cite: 1].
4. Deploy the firmware to a physical ESP32 or import the `diagram.json` and source code into a [Wokwi](https://wokwi.com) simulation project[cite: 1].
5. Ensure network connectivity (or the `Wokwi-GUEST` network if simulating) is active to establish the MQTT handshake with the ThingsBoard broker[cite: 1].
