# ESP-NOW wii controller setup

- **Transmitter**: Reads input from Nintendo controllers and sends data via ESP-NOW.
- **Receiver**: Receives wii controller data.

## Software Setup

### Transmitter

1. Install the following libraries:
   - `ESP8266WiFi`
   - `espnow`
   - `Wire`
   - `NintendoExtensionCtrl`