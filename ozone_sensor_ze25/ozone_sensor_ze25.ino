#include <HardwareSerial.h>
HardwareSerial O3Serial(1);

#define RX_PIN 21       // Sensor TX → ESP32 RX
#define TX_PIN 22       // (optional)
#define ADC_PIN 34      // Analog AOUT input
#define BAUDRATE 9600

void setup() {
  Serial.begin(115200);
  O3Serial.begin(BAUDRATE, SERIAL_8N1, RX_PIN, TX_PIN);
  analogReadResolution(12); // ESP32: 12-bit ADC
  analogSetAttenuation(ADC_11db);
  Serial.println("ZE25-O₃ Sensor Test (UART + Analog)");
}

void loop() {
  // UART data read
  if (O3Serial.available() >= 9) {
    byte buf[9];
    O3Serial.readBytes(buf, 9);
    if (buf[0] == 0xFF && buf[1] == 0x17) {
      int high = buf[4];
      int low = buf[5];
      float ppm_uart = (high * 256 + low) / 1000.0;
      Serial.printf("UART Ozone: %.3f ppm  ", ppm_uart);
    } else {
      Serial.print("Invalid frame ");
    }
  }

  // Analog voltage read
  int adc = analogRead(ADC_PIN);
  float voltage = adc * (3.3 / 4095.0);
  float ppm_analog = (voltage - 0.4) / 0.16;
  if (ppm_analog < 0) ppm_analog = 0;
  if (ppm_analog > 10) ppm_analog = 10;
  Serial.printf("Analog: %.3f V → %.2f ppm\n", voltage, ppm_analog);

  delay(1000);
}
