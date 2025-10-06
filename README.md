# ZE25-O₃ Sensor Interface with ESP32 (UART + Analog)

This project demonstrates how to interface the **Winsen ZE25-O₃ ozone gas sensor** with an **ESP32** using both its **UART (digital)** and **analog** outputs.
The code reads real-time ozone concentration data in **ppm** through UART communication and cross-verifies it using the analog voltage output.

---

## 🧠 Overview

The **ZE25-O₃** sensor provides:

* **UART output** — Digital frame containing gas concentration.
* **Analog output (AOUT)** — Voltage proportional to ozone concentration.

This example reads both simultaneously and prints the results to the serial monitor.

---

## ⚙️ Hardware Setup

| Sensor Pin | Description             | ESP32 Pin |
| ---------- | ----------------------- | --------- |
| VCC        | +5V                     | 5V        |
| GND        | Ground                  | GND       |
| TXD        | UART transmit           | GPIO 21   |
| RXD        | UART receive (optional) | GPIO 22   |
| AOUT       | Analog voltage output   | GPIO 34   |

> ⚠️ The sensor requires 5V power, but its UART TX output is **3.3V compatible**, safe for ESP32 RX.
> A voltage divider may be needed if your module outputs 5V logic levels.

---

## 🧩 Code Explanation

### 1. UART Communication

```cpp
O3Serial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
```

Reads 9-byte data frames from the sensor.
The frame format (from Winsen datasheet):

| Byte | Description                |
| ---- | -------------------------- |
| 0    | Start byte (0xFF)          |
| 1    | Command (0x17)             |
| 4    | High byte of concentration |
| 5    | Low byte of concentration  |
| 8    | Checksum                   |

Concentration is calculated as:

```
ppm = ((High * 256) + Low) / 1000.0
```

### 2. Analog Reading

```cpp
int adc = analogRead(ADC_PIN);
float voltage = adc * (3.3 / 4095.0);
float ppm_analog = (voltage - 0.4) / 0.16;
```

The analog output has:

* **0.4 V → 0 ppm**
* **2.0 V → 10 ppm**

So the slope ≈ 0.16 V/ppm.

---

## 🧪 Output Example

```
ZE25-O₃ Sensor Test (UART + Analog)
UART Ozone: 0.005 ppm  Analog: 0.47 V → 0.43 ppm
UART Ozone: 0.007 ppm  Analog: 0.48 V → 0.50 ppm
```

---

## 🧰 Requirements

* **ESP32** board (any model)
* **Arduino IDE**
* **Winsen ZE25-O₃ sensor**
* Jumper wires and optional breadboard

---

## 🚀 How to Upload

1. Open the `.ino` file in Arduino IDE.
2. Select your ESP32 board and COM port.
3. Upload the code.
4. Open **Serial Monitor** at **115200 baud**.

---

## 📈 Calibration Notes

* Allow the sensor to **warm up for at least 3 minutes** before readings stabilize.
* The analog output may vary slightly between modules — you can adjust the `(voltage - 0.4) / 0.16` formula based on calibration.
* For accurate ppm readings, rely primarily on the **UART output**.

---

## 🪄 License

MIT License — free to use, modify, and share.

---

## 🧑‍💻 Author

**Shinya Kogami**
ESP32 + Sensor Projects | Embedded Systems | IoT Developer

